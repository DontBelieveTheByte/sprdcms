function loadCategory(machineName, humanName) {
    $("#currentCategory").html(humanName).data('machine', machineName);//set a user defined key to hold the machine name
    $("#currentCategory").attr("href", "index.php?q=category/" + $("#currentCategory").data('machine'));//Set the href attribute to open up the link...
    $("#listShirts").html(""); //clear html welcoming the data
    getCategory(machineName);
}
function removeItem(item, name) {
    console.log(name + " " + item);
    $("<div id='question'>You are about to permanently remove " + name + ".</div>").dialog({
        modal: true,
        draggable: false,
        title: "Delete item #" + item + "?",
        resizable: false,
        zIndex: 2000,
        width: 600,
        beforeClose: function(event, ui) {
            $("#question").remove();
        },
        buttons: [
            {
                text: "Cancel",
                click: function() {
                    $(this).dialog("close");
                }
            },
            {
                text: "Yes",
                click: function() {
                    $(this).dialog("close");
                    turtleOn("Removing the item #" + item);
                    var requestData = "r=delShopItem&param=" + item;
                    $.post('index.php?q=ajax', requestData, function(data) {
                        if (data === true) {
                            $("#shirt_" + item).remove();
                            turtleOff();
                        }
                    }, "json");
                    $(this).dialog("close");
                }
            }
        ]
    });
}
function getCategory(category) {
    var requestData = "r=getCategory&param=" + category;
    $.post("index.php?=ajax", requestData, function(data) {
        if (data != false) {
            buildItems(data);
        }
    }, "json");
}
function buildItems(items) {
    $.each(items, function(index, val) {
        var active = (val.active != 1) ? "notActive" : "";//set active button
        $("#listShirts").append(
                "<div class='shirt " + active + "' id='shirt_" + val.id + "'>" +
                "<div class='shirtName'>" + val.name + "</div>" +
                "<img  src='./images/" + val.img1 + "' onmouseover=this.src='./images/" + val.img0 + "' onmouseout=this.src='./images/" + val.img1 + "'>" +
                "<div class='shirtCtrl'>" +
                "<span'>$" + val.price + "</span>" +
                "<span title='Toggle On/Off " + val.name + "' class='icon-off pull-right iconSeperator toggleItem'></span>" +
                "<span title='Delete " + val.name + "' class='icon-trash pull-right iconSeperator articleRemove'></span>" +
                "<span title='Edit description' class='icon-edit pull-right iconSeperator articleDesc'></span>" +
                "<a target='_blank' href='index.php?q=article/" + val.id + "'><span title='View in store' class='icon-eye-open pull-right iconSeperator'></span></a>" +
                '<span id="desc_' + val.id + '" style="display:none">' + val.desc + '</span>' +
                "</div></div>"
                );
        $("#shirt_" + val.id).data('name', val.name);//Storing the names this way is less error prone
        $("#shirt_" + val.id).data('articleId', val.id);
    });
    $("button").button();
    $(".toggleItem").click(function(event) { //Event handler to toggle On/Off an article.
        var parent = $(this).closest(".shirt");
        toggleItem($("#" + parent[0].id).data('articleId'));
    });

    $(".articleRemove").click(function(event) {//Event handler to remove an article from the store.
        var parent = $(this).closest(".shirt");
        removeItem($("#" + parent[0].id).data('articleId'), $("#" + parent[0].id).data('name'));
    });
    $(".articleDesc").click(function(event) {//Event handler to update an article description.
        var parent = $(this).closest(".shirt");
        loadItemDesc($("#" + parent[0].id).data('articleId'));
    });
}
function getCategories() {
    var requestData = "r=getCategories";
    $.post('index.php?q=ajax', requestData, function(data) {
        renderCategories(data);
    }, "json");
}
function renderCategories(categories) {
    $("#categoriesCloud").html('');
    for (category in categories) {
        $("#categoriesCloud").append(
                "<div class='catlist' id='cat_" + category + "\'>" + //The Id is the machine name..
                "<span class='catName' title='Load " + categories[category] + "'>" + categories[category] + "</span>" +
                "<span class='icon-resize-vertical pull-right grab'></span>" +
                "<span id='catdel_" + category + "\' title='Delete " + categories[category] + "' style=' margin-right:1em; cursor:pointer;' class='icon-trash pull-right catdelete'></span>" +
                "</div>");
        $("#cat_" + category).data('human', categories[category]);//Storing the names this way is less error prone
        $("#cat_" + category).data('machine', category);
    }
    $("#categoriesCloud").sortable({
        update: function(event, ui) {
            var requestData = 'r=sortCategories';
            $("#categoriesCloud").children().each(function(index, val) { //Select all the children in the categories cloud and loop to build our POST
                requestData += '&param[]=' + this.id.substr(4);
            });
            turtleOn("Updating the categories order...");
            $.post('index.php?q=ajax', requestData, function(data) {
                if (data === true) {
                } else {
                    ajaxErrorDialog('Internal Server Error [500]. Error sorting categories.');
                }
                turtleOff();
            }, "json");
        },
        opacity: 0.6,
        revert: true,
        delayt: 500
    });
    $(".catlist").click(function(event) { //Event handler to load a category
        loadCategory($("#" + this.id).data('machine'), $("#" + this.id).data('human'));
    });

    $(".catdelete").click(function(event) {//Event handler to delete a category
        deleteCategory($("#" + this.id).closest("div").data('machine'), $("#" + this.id).closest("div").data('human'));
    });
}
function deleteCategory(machineName, humanName) {
    $("<div id='question'>Permanently remove " + humanName + "?</div>").dialog({
        modal: true,
        draggable: false,
        title: "Delete Category",
        resizable: false,
        zIndex: 2000,
        width: 500,
        beforeClose: function(event, ui) {
            $("#question").remove();
        },
        buttons: [
            {
                text: "Cancel",
                click: function() {
                    $(this).dialog("close");
                }
            },
            {
                text: "Yes",
                click: function() {
                    $(this).dialog("close");
                    turtleOn("Deleting category : " + humanName);
                    var requestData = "r=delCategory&param=" + machineName;
                    $.post("index.php?q=ajax", requestData, function(data) {
                        if (data === true) {
                            $("#cat_" + machineName).remove();
                            $("#currentCategory").data('machine') === machineName ? loadCategory("Home", "Home") : false;
                        } else {
                            ajaxErrorDialog('Internal Server Error [500]. Error deleting slected category');
                        }
                        $("#question").remove();
                        turtleOff();
                    }, "json");
                }
            }
        ]
    });
}
function toggleItem(item) {
    var machine = $("#currentCategory").data('machine');
    var requestData = "r=activeToggleItem&param[]=" + machine + "&param[]=" + item;
    turtleOn("Updating " + item + " in " + $("#currentCategory").html());
    $.post("index.php?q=ajax", requestData, function(data) {
        if (data === true) {
            $("#shirt_" + item).toggleClass("notActive");
        }
        turtleOff();
    }, "json");
}
function loadItemDesc(item) {
    var html = '<div id="itemDesc">' +
            '<textarea id="currentDesc" style="width : 98%; height: 95%;">' +
            $("#desc_" + item).html() +
            '</textarea>' +
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
                title: "Update description for item #" + item,
                beforeClose: function(event, ui) {
                    $("#itemDesc").remove();
                },
                buttons: [
                    {
                        text: "Cancel",
                        click: function() {
                            $("#itemDesc").dialog("close");
                        }
                    },
                    {
                        text: "Save",
                        click: function() {
                            updateItemDesc(item, $("#currentDesc").val());
                        }
                    }
                ]
            }
    );
}
function updateItemDesc(item, desc) {
    turtleOn("Updating description for item " + item + ".");
    var requestData = "r=updateItemDesc&param[]=" + item + "&param[]=" + desc;
    $("#desc_" + item).html(desc);
    $.post("index.php?q=ajax", requestData, function(data) {
        if (data === true) {
            turtleOff();
            $("#itemDesc").dialog("close");
        }
    }, "json");
}