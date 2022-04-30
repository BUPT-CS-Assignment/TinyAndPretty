var userid = localStorage.getItem("Userid");
var token = localStorage.getItem("Token");
var ROOT = "http://" + location.host;

function getUserInfo(){
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET",ROOT + "/user/",false);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function", "Fetch");
    xhttp.send();
    if(xhttp.status==200){
        var rec = xhttp.responseText;
        console.log(rec);
        var idx = rec.indexOf("?");
        var code = rec.substring(0,idx);
        var value = rec.substring(idx+1);
        if(code != "0"){
            alert("ErrCode '"+code+"'");
            value = "NULL,NULL,NULL,NULL,NULL,NULL"
        }
        var arr = value.split(",");
        document.getElementById("user_id").innerHTML = arr[0];
        document.getElementById("user_name").innerHTML = arr[1];
        document.getElementById("user_gender").innerHTML = arr[2];
        document.getElementById("user_school").innerHTML = arr[3];
        document.getElementById("user_major").innerHTML = arr[4];
        document.getElementById("user_class").innerHTML = arr[5];
    }else if(xhttp.status == 401){
        alert("ACCESS_DENIED");
        window.open(ROOT+"/signin",'_self');
    }else if(xhttp.status == 400){
        alert("BAD_REQUEST");
    }
}

function Change(){
    document.getElementById("change_info").style.display="block";
    document.getElementById("cover").style.display="block";
}

function SignOut(){
    localStorage.removeItem("Userid");
    localStorage.removeItem("Token");
    alert("Signed Out");
    window.open("http://" + location.host + "/signin",'_self')
}

function updateUserInfo(){
    var xhttp = new XMLHttpRequest();
    /* Get Input Value */
    var input_name = document.getElementById("input_name").value;
    var input_gender = document.getElementById("input_gender").value;
    var input_school = document.getElementById("input_school").value;
    var input_major = document.getElementById("input_major").value;
    var input_class = document.getElementById("input_class").value;
    if(input_name == "" || input_class == ""){
        alert("Empty Input!");
        return;
    }
    console.log(userid);
    if(userid == 10001){
        if(Number(input_class)<0 || Number(input_class)>2147483647){
            alert("Invalid Class Number! Range From 0 to 2147483647.");
            return;
        }
    }else if(input_class.length != 10 || Number(input_class)>2147483647){
        alert("Invalid Class Number! Range From 1000000000 to 2147483647.");
        return;
    }
    xhttp.open("POST",ROOT + "/user/",false);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function", "Update");
    var str = "name="+input_name+",gender="+input_gender+",school="+input_school
            +",major="+input_major+",class="+input_class;
    xhttp.send(str);
    var rec = xhttp.responseText;
    var idx = rec.indexOf("?");
    var code = rec.substring(0,idx);
    var msg = rec.substring(idx+1);
    if(code != "0"){
        alert("ErrCode '"+code+"' "+msg);
        return;
    }
    getUserInfo();
    document.getElementById("change_info").style.display="none";
    document.getElementById("cover").style.display="none";
}

function SignIn(){
    var xhttp = new XMLHttpRequest();
    var ROOT = "http://" + location.host;
    /* Get Input Value */
    var id = document.getElementById("userid").value;
    var pwd = document.getElementById("passwd").value;
    if(id == "" || pwd == ""){
        alert("Empty Input!");
        return;
    }
    if(id+"" == "admin"){
        id = "10000";
    }
    if(Number(id)!=id){
        alert("Invalid UserID!");
        return;
    }
    xhttp.onreadystatechange = function(){
        if(this.readyState == 4){
            if(this.status == 200){
                var rec = this.responseText;
                console.log(this.getAllResponseHeaders());
                var token = this.getResponseHeader("Token");
                localStorage.setItem("Token",token);
                console.log(rec);
                /* Get Response Info */                        
                var pos1 =rec.indexOf("?")
                /* Response Str Split */
                var code = rec.substring(0,pos1);
                var msg = rec.substring(pos1+1);
                /* Info Check */
                if(code != "0"){
                    if(code == "31"){
                        alert("User Unregistered");
                    }else{
                        alert(msg);
                    }
                    return;
                }
                window.open(ROOT+"/user/",'_self');
            }else if(xhttp.status == 400){
                alert("BAD_REQUEST");
            }
        }
    };
    localStorage.setItem("Userid",id);
    xhttp.open("POST",ROOT + "/signin",true);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid",id);
    xhttp.setRequestHeader("Passwd",pwd);
    xhttp.setRequestHeader("Function","signin");
    xhttp.send();
}


function SignUp(){
    var xhttp = new XMLHttpRequest();
    var ROOT = "http://" + location.host;
    /* Get Input Value */
    var id = document.getElementById("userid").value;
    var pwd = document.getElementById("passwd").value + "";
    var cfm = document.getElementById("passwd_cfm").value + "";
    /* Check Valid */
    if(id == "" || pwd == ""){
        alert("Empty Input!");
        return;
    }
    if(Number(id)!=id || id.length!=10 || Number(id)>2147483647){
        alert("Invalid UserID! Range From 1000000000 to 2147483647.");
        return;
    }
    if(pwd.length>16){
        alert("Password Length Overflow!");
        return;
    }
    if(pwd != cfm){
        alert("Passwords Not Match!");
        return;
    }
    /* Action */
    xhttp.onreadystatechange = function(){
        if(this.readyState == 4){
            if(this.status == 200){
                var rec = this.responseText;
                console.log(rec);
                /* Get Response Info */                        
                var pos1 =rec.indexOf("?");
                /* Response Str Split */
                var code = rec.substring(0,pos1);
                var msg = rec.substring(pos1+1);
                /* Info Check */
                if(code != "0"){
                    alert(msg);
                    return;
                }
                alert("Sign Up Success!");
                window.open(ROOT+"/signin",'_self');
            }else if(xhttp.status == 400){
                alert("BAD_REQUEST");
            }
        }
    };
    xhttp.open("POST",ROOT + "/signup",true);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid",id);
    xhttp.setRequestHeader("Passwd",pwd);
    xhttp.setRequestHeader("Function","signup");
    xhttp.send();
}
