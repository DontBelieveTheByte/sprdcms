function importDialog() {
    var html = '<div id="CSVDialog">' +
            '<iframe style="visibility:hidden;display:none" id="derpson" src="index.php?q=getcsv"></iframe>' +
            '<div id=CSVForm>' +
            '<form id="uploadCSVForm" action="" method="post" enctype="multipart/form-data" onsubmit="return false;" >' +
            '<input type="hidden" name="r" value="csvUpload" />' + //We use a hidden form to have custom styles for submit buttons.
            '<input type="hidden" name="MAX_FILE_SIZE" value="100000" />' +
            '<input id="importCSVFile" type="file"  name="csvFile" />' +
            '<button id="uploadCSVButton" type="submit" value="Add" >Import CSV</button>' +
            '</form>' +
            '</div>' +
            '<div id="response"></div>' +
            '<div id="CSVTable"></div>' +
            '</div>';
    $(html).dialog(
            {
                draggable: false,
                minWidth: 900,
                height: window.innerHeight,
                modal: true,
                overflow: "scroll",
                position: "top",
                zIndex: 2000,
                resizable: false,
                closeOnEscape: true, //close function is problematic
                title: "Import shirts from CSV file.",
                beforeClose: function(event, ui) {
                    $("#CSVDialog").remove();
                    loadCategory($("#currentCategory").data('machine'), $("#currentCategory").text());
                },
                buttons: [
                    {
                        text: "Browse",
                        click: function() {
                            $("#importCSVFile").trigger("click").change(function() {
                                $(".ui-dialog-buttonpane").find(".messagespan").html($("#importCSVFile").val().replace('C:\\fakepath\\', ''));
                            });
                        }
                    },
                    {
                        text: "Upload",
                        click: function() {
                            $("#response").html("");
                            $('#uploadCSVForm').ajaxSubmit({//submit the hidden form via ajax
                                url: 'index.php?q=ajax',
                                iframe: true,
                                dataType: 'html',
                                beforeSubmit: turtleOn("Trying to upload CSV file."),
                                success: function() {
                                    loadCSVToTable();
                                    turtleOff();
                                    $(".messagespan").html("Success!");
                                }
                            });
                        }
                    },
                    {
                        text: "Done",
                        click: function() {
                            $("#CSVDialog").dialog("close");
                        }
                    }
                ]
            }
    );
    $(".ui-dialog-buttonpane").append('<div class="messagespan" id="messagespan">Select a file to upload.</div>');
    if ($('#CSVTable').is(':empty')) {
        loadCSVToTable();
    }
}
function loadCSVToTable() {
    $("#CSVTable").remove();
    $("#CSVDialog").append("<div id=CSVTable></div>");//ugly reset but it works
    $('#CSVTable').CSVToTable('index.php?q=getcsv',
            {
                separator: ";",
                trClass: "shirtRow",
                tdClass: "shirtData",
                theadClass: "csvHead"
            }
    ).bind("loadComplete", function() {
        (function beautifulTable() {
            $(".csvHead").remove();//get rid of head
            var removalList = [2, 4, 5, 6, 7, 9, 10, 11, 13]; //the children columns to remove
            var selectorString = "";
            $.each(removalList, function(index, val) {
                selectorString += 'td:nth-child(' + val + '),';
            });
            $(selectorString.slice(0, -1)).hide();//chop that trailing comma and hide
            $("td:nth-child(12)").html(function($this, data) {
                var url = "<img src='" + data + "' />";
                return url;
            });
            $.post("index.php?q=ajax", {r: "getAllIds"}, function(data) {
                var CSVIds = $("td:nth-child(1)"); //retrieve all Ids from CSV constructed table
                var listOfIds = [];
                $.each(data, function(index, val) { //fill array with ids retreived from database
                    listOfIds.push(val.id);
                })
                $.each(CSVIds, function(index, val) { //loop over and cross check from database query results
                    $.each(listOfIds, function(index, idval) {
                        if ($(val).html() == idval) { //if match is found toggle .shirtInStore class
                            $(val).parent().toggleClass("shirtInStore");
                        }
                    })
                });
            }, "json");
        }());
        $(".shirtRow").click(function() { //When we click on a shirt row...
            var currentRow = $(this);
            if (!$(currentRow).hasClass("shirtInStore")) {
                var shirtInfo = [];
                $(this).find('td').each(function() { //fill an array from table info to send via ajax.
                    shirtInfo.push($(this).html());
                });
                turtleOn("Adding a new article to database...");
                $.post("index.php?q=ajax", {r: "addShopItemCSV", param: shirtInfo}, function(data) {
                    turtleOff();
                    if (data === true) {
                        $(currentRow).toggleClass("shirtInStore"); //toggle
                    } else {
                        ajaxErrorDialog("Error adding shirt to database.");
                    }
                }, "json");
            } else {//Give out a warning that shirt is already present in database
                $('<div id="Warning">This item was already added to your store.</div>').dialog({
                    title: "Illegal operation",
                    modal: true,
                    width: "500",
                    draggable: false,
                    position: "center",
                    closeOnEscape: true,
                    resizable: false
                }
                );
            }
        });
    });
    turtleOff();
}