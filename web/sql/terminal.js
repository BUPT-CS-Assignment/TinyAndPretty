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

function getTableList(){
    var xhttp = new XMLHttpRequest();
    var page = ROOT + "/sql";
    xhttp.open("GET",page,false);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function","List");
    xhttp.send();
    if(xhttp.status != 200){
        document.write(xhttp.response);
    }else{
        var str = xhttp.responseText.substring(xhttp.responseText.indexOf("&")+1).split(",");
        for(var i =0;i< str.length;i++){
            var row = document.createElement("tr");
            var line = document.createElement("td");
            var bar = document.createElement("button");
            bar.innerHTML = str[i];
            bar.setAttribute("onclick","getTableInfo(this)");
            line.appendChild(bar);
            row.appendChild(line);
            var table = document.getElementById("table_list");
            table.appendChild(row);
        }
    }
}

function getTableInfo(obj){
    var xhttp = new XMLHttpRequest();
    var page = ROOT + "/sql";
    var table = obj.innerHTML;
    xhttp.open("POST",page,true);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function","Fetch");
    xhttp.send(table);
    xhttp.onreadystatechange = function(){
        if(this.readyState == 4){
            if(this.status == 200){
                document.getElementById("table_info").innerHTML="";
                var caption = document.createElement("caption");
                caption.innerHTML = obj.innerHTML;
                document.getElementById("table_info").appendChild(caption);
                var str = this.responseText.substring(this.responseText.indexOf("&")+1).split(";");
                for(var i = 0; i < str.length;i++){
                    var value = str[i].split(",");
                    var row = document.createElement("tr");
                    for(var j = 0; j < value.length; j++){
                        var line;
                        if(i==0) line =  document.createElement("th");
                        else   line = document.createElement("td");
                        line.innerHTML = value[j];
                        row.appendChild(line);
                    }
                    var table = document.getElementById("table_info");
                    table.appendChild(row);
                }
            }else if(this.status == 401){
                alert("ACCESS_DENIED");
            }else if(xhttp.status == 400){
                alert("BAD_REQUEST");
            }
        }
    };
}
/*
function getTableInfo(){
    var xhttp = new XMLHttpRequest();
    var str = document.getElementById("sql").value;
    if(str == "") return;
    document.getElementById("sql").value = "";
    document.getElementById("req").innerHTML = str;
    document.getElementById("msg").innerHTML = "";
    document.getElementById("val").innerHTML = "";
    xhttp.open("POST",ROOT + "/sql",true);
    xhttp.setRequestHeader("Content-type", "multipart/form-data");
    xhttp.setRequestHeader("Userid", userid);
    xhttp.setRequestHeader("Token",token);
    xhttp.setRequestHeader("Function","Update");
    xhttp.send(str);
    xhttp.onreadystatechange = function(){
        if(this.readyState == 4){
            if(this.status == 200){
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
}*/