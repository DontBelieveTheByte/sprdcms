$(document).ready(function() {
    getCategories();//  render the categories list on page load.
    loadCategory("Home", "Home");
    $("#listShirts").sortable({//Make the shirts sortable.
        update: function(event, ui) {
            var currentCategory = $("#currentCategory").data("machine");//Get the machine name.
            var requestData = 'r=sortCategory&param[category]=' + currentCategory + "&";
            requestData += ($(this).sortable("serialize")).replace(/shirt\[\]/g, 'param[info][]'); //Normalize our POST into a nice multidimensional array.
            turtleOn("Updating the " + currentCategory + " category...");
            $.post('index.php?q=ajax', requestData, function(data) {
                if (data === true) {
                    turtleOff();
                }
            }, "json");
        },
        opacity: 0.6,
        delay: 400,
        revert: true
    });
    $("#addCategoryForm").validate({
        rules: {
            param: {
                required: true,
                minlength: 2,
                maxlength: 20
            }
        },
        messages: {
            param: {
                required: "You must enter a category.",
                minlength: "Minimum of 2 characters.",
                maxlength: "Maximum lenght of 20."
            }
        },
        errorPlacement: function(error, element) {
            error.appendTo($('#categoryError'));
        }
    });
    $("#addCategoryButton").click(function(event) { //adding a category happens here
        if ($("#addCategoryForm").valid()) {
            turtleOn("Adding a new category to database...")
            var requestData = "r=setCategory&" + $("#addCategoryForm").serialize();
            $.post('index.php?q=ajax', requestData, function(data) {
                data ? renderCategories(data) : ajaxErrorDialog('Internal Server Error [500]. Error setting a new category.');
                $("#addCategoryText").val('');
                turtleOff();
            }, "json");
        }
    });
    $("#importButton").click(function(event) {
        importDialog();
    });
    $("button").button();
});