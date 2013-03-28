function settingsPane() {
    var html = '<div id="SettingsPane">' +
            '<div id="tabs">' +
            '<ul>' +
            '<li><a href="#tabs-1">About</a></li>' +
            '<li><a onclick="sprdLog(\'load\');" href="#tabs-2"><span>Log</span></a></li>' +
            '<li><a onclick="sprdCache();" href="#tabs-3"><span>Caching</span></a></li>' +
            '<li><a onclick="sprdCleanURLs(\'load\');" href="#tabs-4"><span>Clean URLs</span></a></li>' +
            '<li><a href="#tabs-5">Backup</a></li>' +
            '</ul>' +
            '<div id="tabs-1">' +
            '<p>SprdCMS is free software under the GPLv3 license. Consider donating if you find it useful or would like to improve it.</p>' +
            '<div id="donate"><a target="blank_" href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2BKSG6H33Z4MA"><img height="100px" alt="Donate with Paypal" border="0" src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif"></a></div>' +
            '<p>Included third party code retain their original license and attribution.</p>' +
            '</div>' +
            '<div id="tabs-2">' +
            '</div>' +
            '<div id="tabs-3">' +
            '</div>' +
            '<div id="tabs-4">' +
            '</div>' +
            '<div id="tabs-5">' +
            '<p>Click the backup link to download a binary dump file of your current database that can be restored with tools such as phpmyadmin.</p></br>' +
            '<div style="width:100%;margin:auto;text-align:center;"><a href="index.php?q=dbBackup"><span style="font-size:4em" class="icon-download"></span><p>Download Backup</p></a></div></br>' +
            '<p>*This will only work if your MySQL server is on the same host as the webserver and PHP has shell access.</p>' +
            '</div>' +
            '</div>';
    $(html).dialog(
            {
                draggable: false,
                minWidth: 900,
                height: 700,
                modal: true,
                overflow: "scroll",
                position: "top",
                zIndex: 2000,
                resizable: false,
                closeOnEscape: true, //close function is problematic
                title: "SprdCMS Settings",
                beforeClose: function(event, ui) {
                    $("#SettingsPane").remove();
                },
                buttons: [
                    {
                        text: "Done",
                        click: function() {
                            $("#SettingsPane").dialog("close");
                        }
                    }
                ]
            }
    );
    $("#tabs").tabs(
            {
                select: function(event, ui) {
                    var position = ui.index;
                    position++;
                    var selectedTab = "tabs-" + position
                    $("[id^=tabs-]").each(function(index, value) {
                        if (selectedTab === value.id) {
                            $("#" + String(value.id)).show();
                        } else {
                            $("#" + String(value.id)).hide();
                        }
                    });
                },
            },
            {
                selected: 0
            });
    $(".settingsBtn").button();
    $("[id^=tabs-]").hide();
    $("#tabs-1").show();
}
function sprdLog(command) {
    switch (command) {
        case "load" :
            $("#tabs-2").html(
                    '<table id="logsTable">' +
                    '</table>' +
                    '</br><button title="Clear the log table" class="settingsBtn" id="logClear" onclick="sprdLog(\'clear\');">Clear log</button>'
                    );
            var requestData = "r=logs&param=getLogs";
            turtleOn("Getting logs from server...");
            $.post("index.php?q=ajax", requestData, function(data) {
                if (data.length > 0) {
                    $(data).each(function(index, value) {
                        $("#logsTable").append(
                                '<tr>' +
                                '<td class="logTime">' + value.time + '<td>' +
                                '<td class="logMsg">' + value.message + '<td>' +
                                '</tr>'
                                );
                    });
                } else {
                    $("#logsTable").append(
                            '<p>There\'s nothing to show for now.</p>'
                            );
                }
                turtleOff();
            }, "json");
            $("button").button();
            break;
        case "clear" :
            var requestData = "r=logs&param=clearLogs";
            turtleOn("Getting logs from server...");
            $.post("index.php?q=ajax", requestData, function(data) {
                if (data === true) {
                    $(data).each(function(index, value) {
                        $("#logsTable").html("Cleared.");
                    });
                }
                turtleOff();
                console.log(data);
            }, "json");
            break;
        default:
            throw "Unrecognized Option";
    }
}
function sprdCache() {
    $("#tabs-3").html(
//            '<p>Cache Time to live (TTL).</p></br>' +
//            '<p>Caching improves performance but will produce a delay for your visitors to notice when modifications are made.</p></br>' +
//            '<div  id="radioTTL">' +
//            '<input id="radio1" type="radio" name="cacheTTL" value="0" /><label for="radio1">&nbsp;Off&nbsp;&nbsp;</label>' +
//            '<input id="radio2" type="radio" name="cacheTTL" value="1" /><label for="radio2"> 5 minutes&nbsp;</label>' +
//            '<input id="radio3" type="radio" name="cacheTTL" value="2" /><label for="radio3"> 1 hour&nbsp;</label>' +
//            '<input id="radio4" type="radio" name="cacheTTL" value="3" /><label for="radio4"> 1 day&nbsp;</label></br></br>' +
//            '</div>' +
            '**This feature is not implimented yet but will be very soon.**</br>' +
            '<button id="cacheTTLUpdate">Save</button>'
            );

    $("#radioTTL").buttonset();
    $("button").button();
    $("#cacheTTLUpdate").click(function() {
        var requestData = "r=cacheUpdate&param=" + $("input[name=cacheTTL]:checked").val();
        $.post("index.php?q=ajax", requestData, function(data) {
            if (data) {
                data.each(function(index, value) {
                    console.log(value);
                })
            }
            turtleOff();
        }, "json");
    });
}
function sprdCleanURLs(command) {
    switch (command) {
        case "load" :
            $("#tabs-4").html(
                    '<p>Clean URLs makes it possible to have better looking URLs for your visitors.</p></br>' +
                    '<span>Normal PHP URL : http://example.com/<b>index.php?q=request</b></span></br>' +
                    '<span>Clean URL : http://example.com/<b>request</b></span>' +
                    '<br><br><span><b>Before you turn it on</b>, click on the test button. </span>' +
                    '<button style="display:inline-block;" onclick="sprdCleanURLs(\'test\')">Test</button><span id="cleanURlsTest"></span>' +
                    '<p>First, make sure you have the <b>mod_rewrite module enabled</b> on your server.</p><br>' +
                    '<p><b>If your site is configured as a virtual host</b> (as it is often the case), you might need to uncomment the following line near the bottom of your <b>.htaccess</b> file located in SprdCMS the root installation folder : </p></br>' +
                    '<pre>#RewriteBase / </pre>' +
                    '<br><p>And make sure it now looks like this : </p><br>' +
                    '<pre>RewriteBase / </pre>' +
                    '<div  id="radio">' +
                    '<input id="radio1" type="radio" name="clean" value="on" /><label for="radio1">On</label>' +
                    '<input id="radio2" type="radio" name="clean" value="off" /><label for="radio2">Off</label>' +
                    '</div>'
                    );
            $("input[type=radio], button").button();
            var requestData = "r=cleanURLs&param=getRewriteStatus";
            turtleOn("Acquiring current clean URLs configuration...");
            $.post("index.php?q=ajax", requestData, function(data) {
                if (data === true) {
                    $("label[for='radio1']").click();
                } else {
                    $("label[for='radio2']").click();
                }
                turtleOff();
            }, "json").done(function() {
                $("input[name='clean']").change(function(event) {
                    sprdCleanURLs($(event.target).val());//Recursive handler on the radio buttons when ajax is done.
                });
            });
            break;
        case "on" :
            var requestData = "r=cleanURLs&param=turnOn";
            turtleOn("Turning on clean URLs...");
            $.post("index.php?q=ajax", requestData, function(data) {
                if (data !== true) {
                    ajaxErrorDialog("Server encountered an internal problem while turning on clean URLs.")
                }
                turtleOff();
                console.log(data);
            }, "json");
            break;
        case "off" :
            var requestData = "r=cleanURLs&param=turnOff";
            turtleOn("Turning off clean URLs...");
            $.post("index.php?q=ajax", requestData, function(data) {
                if (data !== true) {
                    ajaxErrorDialog("Server encountered an internal problem while turning off clean URLs.")
                }
                turtleOff();
                console.log(data);
            }, "json");
            break;
        case "test" :
            turtleOn("Querying rewrite module capability...");
            var url = "http://" + window.location.host + window.location.pathname.replace(/index.php/, "") + "cleanurls";
            console.log(url);
            $.ajax({
                url: url,
                success: function(data) {
                    if (data === true) {
                        $("#cleanURlsTest").html("  <span class='icon-ok'> OK</span>");
                    } else {
                        $("#cleanURlsTest").html("  <span class='icon-remove-circle'>  NOT OK</span>");
                    }
                    turtleOff();
                },
                dataType: "json",
                error: function() {
                    $("#cleanURlsTest").html("  <span class='icon-remove-circle'> NOT OK</span>");
                    turtleOff();
                },
                satusCode: {
                    404: function() {
                        $("#cleanURlsTest").html("  <span class='icon-remove-circle'>  NOT OK</span>");
                        turtleOff();
                    }
                }
            });
            break;
        default:
            throw "Unrecognized Option";
    }
}