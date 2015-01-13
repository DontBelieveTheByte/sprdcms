<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Helper class for the front controller is used to decompose request parameters and transform it into a command with parameters.
 *
 * Helper class for the front controller is used to decompose request parameters and transform it into a command with parameters.
 * The $_GET superglobal is evaluated and exploded into an array. If nothing is passed by $_GET a default command is usually written.
 *
 * @package   SprdCMS
 */
class SprdCMSRequest {

    /**
     * This numerically indexed array that will hold the command and list of parameters.
     * @var array
     */
    protected $parameters = array();

    /**
     * The constructor simply calls a private function to extract the command and parameters.
     */
    public function __construct() {
        $this->extract();
    }

    /**
     * This method does several things to in order to extract a valid command from the $_GET superglobal.
     * It starts by checking if clean URLs are enabled and if it this is the case, it will use $_SERVER['REDIRECT_QUERY_STRING'] as its input.
     * Otherwise it uses the $_GET superglobal and uses a regular expression to explode the query string into an array.
     * The is a special case for handling the testing of clean URLs capability.
     * If nothing can be extracted from the previous cases, it will explicitly return NULL.
     *
     * @return void
     */
    private function extract() {
        if (file_exists('../includes/config.xml') && SprdCMSSiteConfig::getConfig()->getRewriteStatus() && isset($_SERVER['REDIRECT_QUERY_STRING']) && !empty($_SERVER['REDIRECT_QUERY_STRING'])) {
            $this->parameters = explode("/", preg_replace("/^[q][=]/", "", $_SERVER['REDIRECT_QUERY_STRING']));
        } elseif (preg_match("/.+[?q=]/", $_SERVER['REQUEST_URI'])) {
            $this->parameters = explode("/", preg_replace("/.+[?q=]/", "", $_SERVER['REQUEST_URI']));
        } elseif (isset($_SERVER['REDIRECT_QUERY_STRING']) && $_SERVER['REDIRECT_QUERY_STRING'] === 'q=cleanurls') {
            $this->parameters[] = "cleanurls";
        } else {
            $this->parameters[] = NULL;
        }
    }

    /**
     * Getter for the parameters array.
     *
     * @return  array|null Method will return a numerically indexed array with the first index as the command and subsequent indexes as parameters for the command.

     */
    public function getParameters() {
        return $this->parameters;
    }

}

