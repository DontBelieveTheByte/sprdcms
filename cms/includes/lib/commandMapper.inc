<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Transaction script to control most of the flow of execution.
 *
 * A transaction script that resolves requests to commands and also delegates to another SprdCMSProcessor interface object for ajax requests or others.
 *
 * @package SprdCMS
 */
class SprdCMSCommandMapper implements SprdCMSProcessor {

    /**
     * Holds a reference to an instance of SprdCMSViewControl.
     * @var SprdCMSViewController
     */
    protected $viewController;

    /**
     * Holds commands and command parameters.
     * @var array
     */
    protected $parameters;

    /**
     * Accepts an instance of SprdCMSViewControl and assigns an internal reference to it.
     *
     * @param  SprdCMSViewControl A View controller. Needed to handled loading and rendering.
     * @see SprdCMSViewControl
     */
    public function __construct(SprdCMSViewController $viewControl) {
        $this->viewController = $viewControl;
    }

    /**
     * An numerical indexed array is needed as input. The first index is the command to execute and subsequent indexes are parameters.
     * Commands are white listed using a switch block that validates known commands, so any other unknown command is rejected.
     * Implements the SprdCMSProcessor interface, can also delegate requests to other SprdCMSProcessor objects.
     *
     * @see SprdCMSProcessor
     * @param array $params parameters are passed via the front controller's coming from a SprdCMSRequest object. The first array index is must be a whitelisted command or is rejected.
     * @see SprdCMSRequest
     */
    public function process($params = NULL) {
        try {
            $this->parameters = $params;
            switch ($this->parameters[0]) {
                case "install" :
                    if (!file_exists("./includes/config.xml")) {
                        $this->viewController->loadView("install");
                        $this->viewController->render();
                        break;
                    }
                    $this->process("login"); //Recursive call to login as fallback.
                    break;
                case "article":
                case "category":
                    require_once '../includes/lib/storeFront.inc';
                    $store = new SprdCMSStoreFront();
                    $renderable = ($this->parameters[0] === "article") ? $store->getArticle($this->parameters[1]) : $store->getCategory($this->parameters[1]);
                    if (!$renderable) {//in case article or category doesn't exist.
                        header("Location: index.php"); //Redirect to homepage
                    } else {
                        $this->viewController->loadView($this->parameters[0]);
                        $this->viewController->render($renderable);
                    }
                    break;
                case "logout":
                    require_once '../includes/lib/session.inc';
                    SprdCMSSession::stop();
                    $this->process(array("category", "Home"));
                    break;
                case "login":
                    require_once '../includes/lib/session.inc';
                    if (isset($_POST['r']) && $_POST['r'] == "login") {
                        SprdCMSSession::adminLogin($_POST["username"], $_POST['password']);
                    }
                    $view = SprdCMSSession::getLoginStatus() ? "admin" : "login";
                    $this->viewController->loadView($view);
                    $this->viewController->render();
                    break;
                case "admin" :
                    require_once '../includes/lib/session.inc';
                    $view = SprdCMSSession::getLoginStatus() ? "admin" : "login";
                    $this->viewController->loadView($view);
                    $this->viewController->render();
                    break;
                case "getcsv":
                    require_once '../includes/lib/session.inc';
                    if (SprdCMSSession::getLoginStatus() && file_exists('../includes/shirtlist.csv')) {
                        include '../includes/shirtlist.csv';
                    }
                    break;
                case "ajax" :
                    require_once '../includes/lib/ajaxCommandMapper.inc';
                    $ajax = new SprdCMSAjaxController();
                    $ajax->process(); //Delegate to Ajax handler from here.
                    break;
                case "dbBackup" :
                    require_once '../includes/lib/session.inc';
                    if (SprdCMSSession::getLoginStatus() === TRUE) {
                        require_once '../includes/lib/dbBackup.inc';
                        $backup = new SprdCMSDbBackup();
                        $backup->process();
                    } else {
                        throw new Exception("Unauthorized SQL backup download aborted.");
                    }
                    break;
                case "cleanurls" :
                    echo json_encode(TRUE);
                    break;
                default:
                    throw new Exception("Invalid GET parameter " . $this->parameters[0] . " request by " . $_SERVER['REMOTE_ADDR']); //catch exception somewhere later
                    break;
            }
        } catch (Exception $e) {
            $log = new SprdCMSLogger();
            $log->logEvent($e);
            header('HTTP/1.0 404 Not Found');
            echo "<h1>404 Not Found</h1>";
            echo "<p>The page that you have requested could not be found.</p>";
            exit();
        }
    }

}
