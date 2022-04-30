var userid = localStorage.getItem("Userid");
var token = localStorage.getItem("Token");
var ROOT = "http://" + location.host;

function Authenticate(){
    var xhttp = new XMLHttpRequest();
    var page = ROOT + "/sql";
    xhttp.open("GET",page,false);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function","Authenticate");
    xhttp.send();
    if(xhttp.status != 200){
        document.write(xhttp.response);
    }else{
        document.getElementById("cover").style.display="none";
    }
}

function SqlSend(){
    var xhttp = new XMLHttpRequest();
    var str = document.getElementById("sql").value;
    if(str == "") return;
    document.getElementById("sql").value = "";
    document.getElementById("req").innerHTML = str;
    document.getElementById("msg").innerHTML = "";
    document.getElementById("val").innerHTML = "";
    console.log(str);
    xhttp.onreadystatechange = function(){
        if(this.readyState == 4){
            if(this.status == 200){
                var value = "",msg = "",count = "";
                var rec = this.responseText;
                console.log(rec);
                var pos1 =rec.indexOf("?")
                var pos2 = rec.indexOf("&");
                msg = rec.substring(0,pos1);
                count = rec.substring(pos1+1,pos2);
                value = rec.substring(pos2+1);
                if(value == ""){
                    value = "&lt;null&gt;";
                }else if(count > 0){
                    var pos3 = value.indexOf(";");
                    if(pos3 > 0){
                        var str1 = value.substring(0,pos3);
                        var str2 = value.substring(pos3+1);
                        value = "[" + str1 + "]<br>"+str2;
                    }
                }
                document.getElementById("msg").innerHTML = msg;
                document.getElementById("count").innerHTML = count
                document.getElementById("val").innerHTML = value;
            }else if(this.status == 401){
                alert("ACCESS_DENIED");
                window.open(ROOT+"/signin",'_self');
            }else if(xhttp.status == 400){
                alert("BAD_REQUEST");
            }
        }
    };
    var page = "http://" + location.host + "/sql";
    xhttp.open("POST",page,true);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function","SqlRun");
    xhttp.send(str);
}