var ROOT = "http://" + location.host;

function axiosPostWithToken(Url,Param,Val,Function,NoErrFuncton,ErrFunction,ExpFunction){
    axios.post(Url,Val,{
        params:Param,
        headers:{
            'content-type': 'multipart/form-data',
            'function': Function,
            'token':localStorage.getItem('token'),
            'userid':localStorage.getItem('userid')
        }
    })
    .then(res => {
        console.log(res);
        if(res.headers.msg == "NO_ERROR"){
            NoErrFuncton(res);
        }else{
            ErrFunction(res);
        }
    })
    .catch(err => {
        console.log(err);
        ExpFunction(err);
    });
}

function axiosGetWithToken(Url,Param,Function,NoErrFuncton,ErrFunction,ExpFunction){
    axios.get(Url,{
        params:Param,
        headers:{
            'content-type': 'multipart/form-data',
            'function': Function,
            'token':localStorage.getItem('token'),
            'userid':localStorage.getItem('userid')
        }
    })
    .then(res => {
        console.log(res);
        if(res.headers.msg == "NO_ERROR"){
            NoErrFuncton(res);
        }else{
            ErrFunction(res);
        }
    }) 
    .catch(err => {
        console.log(err);
        ExpFunction(err);
    })
}