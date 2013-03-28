<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>SprdCMS Administrative Page</title>
        <script src="misc/js/jquery-1.9.1.min.js"></script>
        <script src="misc/jquery-ui-1.10.2.custom/js/jquery-ui-1.10.2.custom.min.js"></script>
        <script src="misc/js/jquery.form.js"></script>
        <script src="misc/js/jquery.validate.min.js"></script>
        <script src="misc/js/jquery.csvToTable.js"></script>
        <script src="misc/js/ajaxerr.js"></script>
        <script src="misc/js/csvImport.js"></script>
        <script src="misc/js/turtle.js"></script>
        <script src="misc/js/store.js"></script>
        <script src="misc/js/admin.js"></script>
        <script src="misc/js/settingsPane.js"></script>
        <link rel="stylesheet" type="text/css" href="misc/css/cssreset-min.css" />
        <link rel="stylesheet" href="misc/jquery-ui-1.10.2.custom/css/custom-theme/jquery-ui-1.10.2.custom.min.css" type="text/css" />
        <link rel="stylesheet" href="misc/css/admin.css" type="text/css" />
        <link rel="stylesheet" href="misc/css/admin.css" type="text/css" />
        <link rel="stylesheet" href="misc/css/font-awesome.min.css">
        <link rel="shortcut icon" href="misc/favicon.png">
        <link rel="icon" href="misc/favicon.png">
    </head>
    <body>
        <span class="currentCategory">Category : <a target="_blank" href="index.php" id="currentCategory">Home</a></span>
        <div id="container">
            <div id="adminPannel">
                <div style="padding-bottom: 25px;" class="adminPannelForm">
                    <span style="padding-top: .5em;" class="pull-left" style="">Add new shirts.</span>
                    <span class="pull-right"><button style="display: inline-block;" id="importButton">Import from CSV</button></span>
                </div>
                </br></br>
                <div class="adminPannelForm">
                    <form id="addCategoryForm" onsubmit="return false;">
                        <input  id="addCategoryText" class="inputfield" type="text" placeholder="category" size="15" name="param" />
                        <button style="display: inline-block;" class="pull-right" id="addCategoryButton" name="submit" value="Add">Add</button>
                        <div class="validateError" id="categoryError"></div>
                    </form>
                </div>
                </br>
                <div id="mainCat">
                    <div title="Load Home" onClick = 'loadCategory("Home", "Home");' class='catlist' id='Home'>
                        <span class="catName">Home</span>
                        <span class="icon-home pull-right"></span>
                    </div>
                </div>
                <div id="categoriesCloud"></div>
                <div id="logout"><span><a href="index.php?q=logout">Logout</a></span>
                    <span style="margin-right: 1em" title='Settings' class="icon-wrench pull-right" style='cursor:pointer;' onClick ="settingsPane();"></span>
                </div>
            </div>
            <div id='listShirts'></div>
        </div>
    </body>
</html>