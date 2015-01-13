<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * This class is in charge of enabling or disabling URL rewriting.
 *
 * @package   SprdCMS
 */
class SprdCMSURLRewrite {

    /**
     * This property holds the textual content of a configuration file.
     *
     * @var type
     */
    private $config;

    /**
     * The constructor loads the configuration file as text, not as XML.
     *
     * @todo    Add some error checking.
     */
    public function __construct() {
        $this->config = @file_get_contents('../includes/config.xml');
    }

    /**
     * Get the current value of the rewrite status from the configuration file.
     *
     * @return bool Will return TRUE or FALSE depending of the current state.
     */
    public function getRewriteStatus() {
        return SprdCMSSiteConfig::getConfig()->getRewriteStatus();
    }

    /**
     * Turn on URL rewriting to work with Mod Rewrite.
     * Since the structure of the document does not change,
     * we use a regular expression to modify the configuration file.
     *
     * @return bool Returns TRUE or FALSE on success or failure to change the configuration file.
     */
    public function turnOn() {
        try {
            $isURLRewriteOn = preg_replace('/(<modrewrite>0<\/modrewrite>)/m', '<modrewrite>1</modrewrite>', (String) $this->config);
            if (@file_put_contents('../includes/config.xml', $isURLRewriteOn) !== FALSE) {
                return TRUE;
            }
            throw new Exception("Cannot turn on Mod Rewrite functionality. Make sure mod_rewrite is enabled.");
        } catch (Exception $e) {
            $logger = new SprdCMSLogger();
            $logger->logEvent($e);
            return FALSE;
        }
    }

    /**
     * Turn off URL rewriting to work with Mod Rewrite.
     * Since the structure of the document doesn't change,
     * we use a regular expression to modify the configuration file.
     *
     * @return bool Returns TRUE or FALSE on success or failure to change the configuration file.
     */
    public function turnOff() {
        try {
            $isURLRewriteOn = preg_replace('/(<modrewrite>1<\/modrewrite>)/m', '<modrewrite>0</modrewrite>', (String) $this->config);
            if (@file_put_contents('../includes/config.xml', $isURLRewriteOn) !== FALSE) {
                return TRUE;
            }
            throw new Exception("Problem while turning off Mod Rewrite functionality.");
        } catch (Exception $e) {
            $log = new SprdCMSLogger();
            $log->logEvent($e);
            return FALSE;
        }
    }

}

