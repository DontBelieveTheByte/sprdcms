function ajaxErrorDialog(notVeryHelpfulMessage) {
    $("<div id='ajaxError'>" + notVeryHelpfulMessage + "<br><br>Try the last operation again, refreshing the page or check the connection state with the server.</div>").dialog({
        title: "Oh no! Server error...",
        modal: true,
        width: 800,
        draggable: false,
        resizable: false,
        position: "top"
    });
}
$(document).ready(
        $.ajaxSetup({
    error: function(jqXHR, exception) {
        if (jqXHR.status === 0) {
            ajaxErrorDialog('Not connect.\n Verify Network.');
        } else if (jqXHR.status == 404) {
            ajaxErrorDialog('Requested page not found. [404]');
        } else if (jqXHR.status == 500) {
            ajaxErrorDialog('Internal Server Error [500].');
        } else if (exception === 'parsererror') {
            ajaxErrorDialog('Requested JSON parse failed.');
        } else if (exception === 'timeout') {
            ajaxErrorDialog('Time out error.');
        } else if (exception === 'abort') {
            ajaxErrorDialog('Ajax request aborted.');
        } else {
            ajaxErrorDialog('Uncaught Error.\n' + jqXHR.responseText);
        }
    }
})

        );