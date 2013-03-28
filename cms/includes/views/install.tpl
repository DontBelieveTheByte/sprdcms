
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>SprdCMS Installation</title>
        <link rel="shortcut icon" href="misc/favicon.png">
        <link rel="icon" href="misc/favicon.png">
        <link rel="stylesheet" type="text/css" href="misc/css/cssreset-min.css" />
        <link rel="stylesheet" href="misc/jquery-ui-1.10.2.custom/css/custom-theme/jquery-ui-1.10.2.custom.min.css" type="text/css" />
        <link rel="stylesheet" href="misc/css/admin.css" type="text/css" />
        <script src="misc/js/jquery-1.9.1.min.js"></script>
        <script src="misc/jquery-ui-1.10.2.custom/js/jquery-ui-1.10.2.custom.min.js"></script>
        <script src="misc/js/jquery.validate.min.js"></script>
        <script src="misc/js/install.js"></script>
        <script src="misc/js/ajaxerr.js"></script>
        <script src="misc/js/turtle.js"></script>
    </head>
    <body>
        <div id="initContainer">
            <h1>Welcome to the SprdCMS installer</h1><br/>
            <div class="regform">
                <form id="initForm" method="POST" onsubmit="return false;">
                    <h2>Your Shop Information</h2>
                    <input class="inputfield" id="shopName" type="text" name="shopName" placeholder="Shop name" autofocus="autofocus" ><br>
                    <input class="inputfield" id="shopId" type="text" name="shopId" placeholder="Shop ID"><br>
                    <div  id="radio">
                        <input id="radio1" type="radio" name="domain" value="com" /><label for="radio1">.com</label>
                        <input id="radio2" type="radio" name="domain" value="net" /><label for="radio2">.net</label>
                        <span id="domainHelper">Select a domain for American or European store.</span>
                    </div>
                    <h2>CMS Login Information</h2>
                    <input class="inputfield" id="username" type="text" name="username" placeholder="Username"> <span class="helperTxt"></span><br/>
                    <input class="inputfield" id="password" type="password" name="password" placeholder="Password"> <div id="progressBar"></div><br>
                    <input class="inputfield" type="password" name="confirmPassword" placeholder="Repeat password"><div id="match"></div><br/>
                    <input class="inputfield" id="email" type="text" name="email" placeholder="Email address"><br>
                    <input class="inputfield" type="text" name="mailcheck" placeholder="Confirm email"><br>
                    <h2>MySQL Database information</h2>
                    <input class="inputfield" id="dbName" type="text" name="dbName" placeholder="Database name"><br>
                    <input class="inputfield" id="dbUser" type="text" name="dbUser" placeholder="Database user"><br>
                    <input class="inputfield" id="dbPass" type="password" name="dbPass" placeholder="Database password"><br>
                    <input class="inputfield" id="dbHost" type="text" name="dbHost" placeholder="localhost" value="localhost"><br>
                    <input class="inputfield" id="dbPort" type="text" name="dbPort" placeholder="default" value="3306"> <br>
                    <br/>
                    <span style="float:right"><input id="initSubmit" type="button"  name="submit" value="submit"></span>
                </form>
            </div>
            <div class="cmslogo">
                <object  data="misc/cms_logo.svg" type="image/svg+xml"></object>
                <ol style="list-style: upper-roman" >
                    <li class="initList" >Create a MySQL database.</li>
                    <li class="initList" >Fill out this form with your information and complete the installation.</li>
                    <li class="initList" >Customize the look and feel by adjusting the template markup and CSS.</li>
                    <li class="initList" >Start organizing your store by creating categories and adding item from a CSV file.</li>
                    <li class="initList" >???</li>
                    <li class="initList" >Profit!</li>
                </ol>
            </div>
        </div>
    </body>
</html>