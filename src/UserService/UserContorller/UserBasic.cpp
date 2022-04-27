#include <UserService/UserControl.h>
#include <UserService/Preload.h>

User::User(){
    id_ = 0;
    auth_ = 0;
    status_ = 0;
    UserDB = NULL;
}

User::User(int id, int auth){
    id_ = id;
    auth_ = auth;
    UserDB = NULL;
    status_ = USER_SIGN_OUT;
}

int User::SignIn(std::string passwdInput){
    std::string id_s = std::to_string(id_);
    // Signing In //
    std::string sql = "select Passwd from UserToken where ID = " + std::to_string(id_) + ";";
    MainDB.Exec(sql.c_str());
    int errCode = MainDB.ErrCode();
    if(errCode != 0){
        return errCode;
    }
    std::string val = MainDB.ReturnVal();
    val = val.substr(val.find(";") + 1);
    if(passwdInput != val){
        return 31;
    }
    status_ = USER_SIGN_IN;
    std::string userDir = USER_DIR + "/" + id_s;
    if(UserDB == NULL){
        UserDB = new NEDB(userDir.c_str());
        UserDB->DirInit();
    }
    return CONSOLE_LOG(0, "User '%d' Signed In\n", id_);
}

int User::SignUp(std::string passwdInput){
    std::string id_s = std::to_string(id_);
    std::string sql = "insert into UserToken values (" + id_s + "," + passwdInput + ",0);";
    sql += "insert into UserInfo (ID) values (" + id_s + ");";
    MainDB.Exec(sql.c_str());
    if(MainDB.ErrCode() == 0){
        std::string userDir = USER_DIR + "/" + id_s;
        mkdir(userDir.c_str(), S_IRWXU);
        CONSOLE_LOG(0, "User Dir '%s' Created\n", userDir.c_str());
    }
    return MainDB.ErrCode();
}

int User::SignOut(){
    status_ = USER_SIGN_OUT;
    UserDB->Close();
    UserDB = NULL;
    COOKIE.DeleteData(id_);
    return 0;
}

std::string User::GetAllInfo(){
    std::string sql = "select * from UserInfo where ID = " + std::to_string(id_) + ";";
    /*
    NEDB TempDB(PRELOAD_DIR.c_str());
    TempDB.Open("UserInfo");
    TempDB.Exec(sql.c_str());
    std::string res = TempDB.ReturnVal();
    empDB.Close();
    return res;
    */
    MainDB.Exec(sql.c_str());
    std::string res = MainDB.ReturnVal();
    res = res.substr(res.find(";") + 1);
    int length;
    string* str = Split(res, ',', length);

    /* Return ID and Name */
    string resp = str[0] + "," + str[1] + ",";

    /* Return Gender */
    if(str[2] == "0") resp += "女,";
    else resp += "男,";

    /* Return School */
    sql = "select Name from SchoolInfo where ID = " + str[3] + ";";
    MainDB.Exec(sql.c_str());
    res = MainDB.ReturnVal();
    resp += res.substr(res.find(";") + 1) + ",";

    /* Return Major */
    sql = "select Name from MajorInfo where ID = " + str[4] + ";";
    MainDB.Exec(sql.c_str());
    res = MainDB.ReturnVal();
    resp += res.substr(res.find(";") + 1) + ",";

    /* Return Class */
    resp += str[5];

    return resp;
}

int User::SetInfo(std::string value){
    std::string sql = "update UserInfo set " + value + " where ID = " + std::to_string(id_) + ";";
    return MainDB.Exec(sql.c_str());
}

std::string User::GetMajor(){
    std::string sql = "select Major from UserInfo where ID = " + std::to_string(id_) + ";";
    if(MainDB.Exec(sql.c_str()) != 0){
        return "ERROR";
    }
    std::string val = MainDB.ReturnVal();
    val = val.substr(val.find(";") + 1);
    return val;
}

std::string User::GetGender(){
    std::string sql = "select Gender from UserInfo where ID = " + std::to_string(id_) + ";";
    if(MainDB.Exec(sql.c_str()) != 0){
        return "ERROR";
    }
    std::string val = MainDB.ReturnVal();
    if(val[val.length() - 1] == '1'){
        return "Female";
    }
    return "Male";
}

int User::GetClass(){
    std::string sql = "select Class from UserInfo where ID = " + std::to_string(id_) + ";";
    if(MainDB.Exec(sql.c_str()) != 0){
        return 0;
    }
    std::string val = MainDB.ReturnVal();
    val = val.substr(val.find(";") + 1);
    return stoi(val);
}

std::string User::GetName(){
    std::string sql = "select Name from UserInfo where ID = " + std::to_string(id_) + ";";
    if(MainDB.Exec(sql.c_str()) != 0){
        return "ERROR";
    }
    std::string val = MainDB.ReturnVal();
    val = val.substr(val.find(";") + 1);
    return val;
}