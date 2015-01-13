function turtleOn(message) {
    $("<div id='turtle'><span class='loader' style='display:inline-block'></span> " + message + "</div>").dialog({
        title: "Waiting for database...",
        modal: true,
        width: "500",
        draggable: false,
        position: "center",
        zIndex: "3999",
        closeOnEscape: false,
        resizable: false
    });
    $("#turtle").parent().find("button").remove();
}
function turtleOff() {
    $("#turtle").remove();
}