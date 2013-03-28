<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * This class is in charge of handling all the ajax request.
 *
 * This class is in charge of handling all the ajax request through delegation by the SprdCMSCommandMapper.
 *
 * @see CommandMapper
 * @package   SprdCMS
 */
class SprdCMSAjaxController implements SprdCMSProcessor {

    /**
     * Holds values of $_POST['r'] that should resolve commands.
     * @var string
     */
    protected $command;

    /**
     * Holds values of $_POST['param'] that should resolve into command parameters.
     * @var array
     */
    protected $storeParam = NULL;

    /**
     * Contructor sets command and command parameters from the $_POST superglobal.
     *
     * @return  void
     */
    public function __construct() {
        $this->command = $_POST['r'];
        $this->storeParam = (isset($_POST['param']) && !empty($_POST['param'])) ? $_POST['param'] : NULL;
    }

    /**
     * Commands are whitelisted using a switch block that validates known commands, so any other unkown command is rejected.
     * Implements the SprdCMSProcessor interface, can also delegate requests to other SprdCMSProcessor objects.
     *
     * @see SprdCMSProcessor
     * @param array $params parameters are passed via the front controller's comming from a SprdCMSRequest object. The first array index is must be a whitelisted command or is rejected.
     * @see SprdCMSRequest
     */
    public function process($param = NULL) {
        try {
            require_once './includes/lib/session.inc';
            switch ($this->command) {
                case "activeToggleItem" :
//                case "addShopItemManual" : Not implemented yet
                case "addShopItemCSV" :
                case "delCategory" :
                case "delShopItem" :
                case "getAllIds" :
                case "getCategories" :
                case "getCategory" :
                case "logs" :
                case "setCategory" :
                case "sortCategory" :
                case "sortCategories" :
                case "updateItemDesc" :
                    if (SprdCMSSession::getLoginStatus()) {
                        require_once './includes/lib/backStore.inc';
                        $store = new SprdCMSBackStore();
                        echo json_encode($store->{$this->command}($this->storeParam));
                    } else {
                        throw new Exception("Unauthorized Ajax request $this->command by IP " . $_SERVER['REMOTE_ADDR']);
                    }
                    break;
                case "install" :
                    if (file_exists("./includes/config.xml")) {
                        header("HTTP/1.1 500 Internal Server Error"); //Stop against request forgery once installed.
                        break;
                    }
                    require_once './includes/lib/install.inc';
                    $installer = new SprdCMSInstall();
                    echo json_encode($installer->initStore());
                    break;
                case "csvUpload" :
                    if (SprdCMSSession::getLoginStatus() && isset($_FILES['csvFile']) && $_FILES['csvFile']['type'] == "text/csv") {
                        echo json_encode(copy($_FILES['csvFile']['tmp_name'], "./includes/shirtlist.csv"));
                    } else {
                        throw new Exception("IP " . $_SERVER['REMOTE_ADDR'] . " failed to upload and save CSV File " . $_FILES['csvFile']);
                    }
                    break;
                case "cleanURLs":
                    if (SprdCMSSession::getLoginStatus()) {
                        $rewrite = new SprdCMSRewrite();
                        echo json_encode($rewrite->{$this->storeParam}());
                    } else {
                        throw new Exception("Unauthorized Ajax request $this->command by IP " . $_SERVER['REMOTE_ADDR']);
                    }
                    break;
                default :
                    header("HTTP/1.1 500 Internal Server Error");
                    throw new Exception("General Ajax error on request $this->command from IP " . $_SERVER['REMOTE_ADDR']);
                    break;
            }
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            exit();
        }
    }

}

?>