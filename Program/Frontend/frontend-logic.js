function displayQueryOnHTML(queryResult){
    var queryFieldList = document.getElementById('queryFieldList');
    if(queryResult == null){
        var queryFieldListItem = document.createElement('li');
        queryFieldListItem.innerHTML = 'Error: no se recibieron datos de la query';
        queryFieldList.appendChild(queryFieldListItem);
    }else{
        for(const [key,value] of Object.entries(queryResult)){
            var queryFieldListItem = document.createElement('li');
            queryFieldListItem.innerHTML = `${key}: ${value}`;
            queryFieldList.appendChild(queryFieldListItem);
        }
    }
}

document.addEventListener('DOMContentLoaded', function(){
    fetch('query-fetcher.php')
    .then(response => response.json())
    .then(data => {
        console.log(data.queryResult);
        displayQueryOnHTML(data.queryResult);
    });
    
})