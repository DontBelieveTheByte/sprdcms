<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * This class handles finding and loading and redenring of different views.
 *
 * The constructor of the User class takes a Data Mapper which
 * handles the user persistence in the database. In this case,
 * we will provide a fake one.
 *
 * @package   SprdCMS
 */
class SprdCMSViewControl {

    /**
     * This holds the full path along with the filename of the template view that needs to be loaded.
     * @var string
     */
    protected $template;

    /**
     * All views are located in includes/views/*.tpl. The string parameter will be matched against a file template on disk.
     *
     * @param string $view The name of the view to load.
     * @throws Exception If a requested view doesn't exist, this failed attempt will be logged and method will return false.
     * @return  void
     */
    public function loadView($view) {
        try {
            if (file_exists("./includes/views/$view.tpl")) {
                $this->template = "./includes/views/$view.tpl";
            } else {
                throw new Exception("Cannot locate and load HTML template file $view. Template file is either missing or file permission if wrong.");
            }
        } catch (Exception $exc) {
            $log = new Logger();
            $log->logEvent($exc);
            echo $exc->getMessage();
            exit();
        }
    }

    /**
     * This method takes care of the actual rendering. The loaded template is included and passed in a renderable array.
     *
     * @param array $renderable An associate array with all that is needed to render the view correctly.
     * @return void
     * @throws Exception Failed to include a template will trigger an exception that will log this event.
     */
    public function render(Array $renderable = NULL) {
        try {
            if (!require_once ($this->template)) {
                throw new Exception("Unable to include and render view $this->template.");
            }
        } catch (Exception $exc) {
            $log = new Logger();
            $log->logEvent($exc);
        }
    }

}

?>
