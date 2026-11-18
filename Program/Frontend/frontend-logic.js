function displayQueryOnHTML(queryResult){
    var queryFieldList = document.getElementById('queryFieldList');
    if(queryResult == null){
        var queryFieldListItem = document.createElement('li');
        queryFieldListItem.innerHTML = 'Error: no se recibieron datos de la query';
        queryFieldList.appendChild(queryFieldListItem);
    }else{
        queryResult.forEach((row) => {
            var queryFieldListItem = document.createElement('li');
            queryFieldListItem.innerHTML = `Nombre comedor: ${row.nombre}, Capacidad máxima: ${row.capacidad_maxima}`;
            queryFieldList.appendChild(queryFieldListItem);
        });
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