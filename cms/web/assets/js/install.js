$(document).ready(function() {
    // progress bar for the password field
    $("#password").keyup(function() {
        var len = $(this).val().length;
        (function(prog) {
            $("#progressBar").css("clear", "none").css("float", "right").progressbar({
                value: (prog * 100) / 8
            })
        })(len);
    });
    $("#initSubmit").button();
    $("#radio").buttonset();
    //Validate the form client side
    $("#initForm").validate({
        debug: false,
        rules: {
            shopName: {
                required: true,
                minlength: 2,
                maxlength: 50
            },
            shopId: {
                required: true,
                digits: true,
                minlength: 6,
                maxlength: 6
            },
            domain: {
                required: true
            },
            username: {
                required: true,
                minlength: 3,
                maxlength: 35
            },
            password: {
                required: true,
                minlength: 8,
                maxlength: 50
            },
            confirmPassword: {
                required: true,
                equalTo: "#password"
            },
            email: {
                required: true,
                email: true
            },
            mailcheck: {
                required: true,
                equalTo: '#email'
            },
            dbName: {
                required: true

            },
            dbUser: {
                required: true
            },
            dbPass: {
                required: true
            },
            dbHost: {
                required: true
            },
            dbPort: {
                required: true
            }
        },
        messages: {
            shopName: {
                required: "What's the name of your shop?"
            },
            shopId: {
                required: "Enter your 6 digits shop ID.",
                digits: "Enter your 6 digits shop ID.",
                minlength: "Enter your 6 digits shop ID.",
                maxlength: "Enter your 6 digits shop ID."
            },
            domain: {
                required: function() {

                }
            },
            username: {
                required: "Pick a name for administrative purposes."
            },
            password: {
                required: "Think of a good login password.",
                minlength: "At least 8 characters.",
                maxlength: "Maximum 50 characters."
            },
            confirmPassword: {
                required: "Confirm your password.",
                equalTo: "Confirm your password."
            },
            email: {
                required: "If you forget your password I will email you."
            },
            mailcheck: {
                required: "Verify your email address.",
                equalTo: 'Your email must match.'
            },
            dbName: {
                required: "Name of MySQL database SprdCMS will use."
            },
            dbUser: {
                required: "The name of your MySQL database user."
            },
            dbPass: {
                required: "Your MySQL user password."
            },
            dbHost: {
                required: "Change only if MySQL is on different host."
            },
            dbPort: {
                required: "Default port is 3306 or enter your own."
            }
        }
    });
    //Submit handler
    $("#initSubmit").click(function(event) {
        event.preventDefault();
        if ($("#initForm").valid() === false) { //Stop the user from submitting if form is incorrect.
            $("<div id='verify'>You must fill all the required fields</div>").dialog(
                    {
                        buttons: [
                            {
                                text: "ok",
                                click: function() {
                                    $(this).dialog("close");
                                    $("#verify").remove();
                                }
                            }
                        ],
                        title: "Fill out the form correctly",
                        width: 800,
                        modal: true,
                        draggable: false,
                        hide: "slideToggle",
                        show: "slideToggle",
                        resizable: false,
                        position: "top"
                    });
            $(".ui-icon-closethick").parent().remove();
        } else {//Proceed if form is valid.
            $("<div id='verify'></div>").dialog(//Gives the user a chance to review the settings.
                    {
                        buttons: [
                            {
                                text: "Go back",
                                click: function() {
                                    $(this).dialog("close");
                                    $("#verify").remove();
                                }
                            },
                            {
                                text: "I'm sure!",
                                click: function() {
                                    var requestData = "r=install&" + $("#initForm").serialize();
                                    turtleOn("Activating your site...");
                                    $.post('index.php?q=ajax', requestData, function(data) { //If user is sure, make ajax call.
                                        if (data === "true") { //Success handler.
                                            turtleOff();
                                            $("<div id='postInstall'>You may now <a style='text-decoration:underline;' href='index.php'>visit your site</a> and customize the CSS in the themes folder or <a style='text-decoration:underline;' href='index.php?q=login'>login</a> to the administration page.</div>").dialog({
                                                title: "Success!",
                                                modal: true,
                                                width: 800,
                                                draggable: false,
                                                hide: "slideToggle",
                                                show: "slideToggle",
                                                resizable: false,
                                                position: "top"
                                            });
                                            $(".ui-icon-closethick").parent().remove();
                                        } else { //Failure handler.
                                            turtleOff();
                                            $("<div id='postInstall'>" + data + "<br />Looks like something went wrong.<br /> Verify your installation requirements, read the FAQ or send a bug report.</div>").dialog({
                                                title: "Bummer",
                                                modal: true,
                                                width: 800,
                                                draggable: false,
                                                hide: "slideToggle",
                                                show: "slideToggle",
                                                resizable: false,
                                                position: "top"
                                            });
                                            $(".ui-icon-closethick").parent().remove();//Disables the annoying X button.
                                        }
                                    }, ""); //datype is intentionally left out to let JQuery guess the datatype, either returns json true or text
                                }
                            }
                        ],
                        beforeClose: function(event, ui) {
                            $("#verify").remove();
                        },
                        title: "Verify your settings",
                        width: 900,
                        modal: true,
                        draggable: false,
                        resizable: false,
                        position: "top"
                    }
            );
            $("#verify").append(function() {//Displays *** instead of clear password on screen for spying eyes.
                var passMask = function(len) {
                    var mask = "";
                    for (var i = 0; i < len; i++) {
                        mask = mask + '*';
                    }
                    return mask;
                };
                return 'Shop name : <b>' + $("#shopName").val() + '</b><br />' +
                        'Shop ID : <b>' + $("#shopId").val() + '</b><br />' +
                        'Domain : Spreadshirt<b>.' + $("input[name='domain']:checked").val() + '</b><br />' +
                        'Username : <b>' + $("#username").val() + '</b><br />' +
                        'Password : <b>' + passMask($("#password").val().length) + '</b><br />' +
                        'Email : <b>' + $("#email").val() + '</b><br />' +
                        'MySQL database : <b>' + $("#dbName").val() + '</b><br />' +
                        'MySQL username : <b>' + $("#dbUser").val() + '</b><br />' +
                        'MySQL password : <b>' + passMask($("#dbPass").val().length) + '</b><br />' +
                        'Database hotsname : <b>' + $("#dbHost").val() + '</b><br />' +
                        'Database port : <b>' + $("#dbPort").val() + '</b><br />';
            });
        }
    });
    $("#initForm").valid();
});