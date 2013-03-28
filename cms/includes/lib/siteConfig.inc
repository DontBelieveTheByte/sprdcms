<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Provides application wide configuration values.
 *
 * A helper class that is used to provide application wide configuration values as a registry of run time settings.
 * Most configuration values are loaded from an XML file located at includes/config.xml.
 * Implemented as a singleton to use a single configuration.
 * There is no locking mechanism since only a single user can modify the configuration.
 *
 * @package SprdCMS
 */
class SprdCMSSiteConfig {

    /**
     * An instance of the current class type.
     * @var SprdCMSSiteConfig
     */
    private static $instance;

    /**
     * Holds the base path to make URLs rewriting possible.
     * @var string
     */
    private static $basePath;

    /**
     * Information needed to construct a DSN (data source name) in order to connect to the database.
     * @var array
     */
    private $connectStr;

    /**
     * Hold the XML configuration file in order to access its properties.
     * @var SimpleXML
     */
    private $xml;

    /**
     * The construtor's role is to load the XML configuration file as a SimpleXML object.
     * @return  void
     * @throws  Exception Failure to initialize a DSN connection string will trigger an exception but cannot be logged to the database for evident reason.
     */
    private function __construct() {
        try {
            if (!$this->xml = @simplexml_load_file("./includes/config.xml")) {
                throw new Exception("Not able to load XML configuration file.</br>You might need to <a href='index.php?q=install'>run the installer</a>.</br>");
            }
            $this->connectStr = array("mysql:host=" . $this->xml->db->host . ";port=" . $this->xml->db->port . ";dbname=" . $this->xml->db->name, $this->xml->db->username, $this->xml->db->password);
        } catch (Exception $e) {
            echo $e->getMessage();
            exit();
        }
    }

    /**
     * Since the constructor is private we use this method in order to get an instance.
     * It stores the instance inside of a private variable and returns the same instance
     * each time a call to this method is being made.
     *
     * @static
     * @return SprdCMSSiteConfig Returns an instance of the current class.
     */
    public static function getConfig() {
        if (!isset(self::$instance)) {
            self::$instance = new SprdCMSSiteConfig();
            return self::$instance;
        } else {
            return self::$instance;
        }
    }

    /**
     * Helper function that returns an array containing information needed to build a DSN (data source name).
     *
     * @return array Retuns an array of strings with all the database credentials needed to construct PDO.
     * @see SprdCMSDb
     */
    public function getdbCredentials() {
        return $this->connectStr;
    }

    /**
     * Helper function to return store name.
     *
     * @return string Returns the human readable name of the store.
     */
    public function getStoreName() {
        return (string) $this->xml->shop->store;
    }

    /**
     * Helper function used by the SprdCMSDbBackup class.
     * @see SprdCMSDbBackup
     *
     * @return string The name of the database to connect to.
     */
    public function getDbName() {
        return (string) $this->xml->db->name;
    }

    /**
     * Helper function used by the SprdCMSDbBackup class.
     * @see SprdCMSDbBackup
     *
     * @return string The username of the database to connect to.
     */
    public function getDbuser() {
        return (string) $this->xml->db->name;
    }

    /**
     * Helper function used by the SprdCMSDbBackup class.
     * @see SprdCMSDbBackup
     *
     * @return string The password of the database to connect to.
     */
    public function getDbPass() {
        return (string) $this->xml->db->password;
    }

    /**
     * Helper function for getting the Spreadshit shop unique ID. Useful for querying the Spreadshirt API.
     * @see SprdCMSItem
     *
     * @return string Returns the ID of the shop with the value used as input during installation.
     */
    public function getShopID() {
        return (string) $this->xml->shop->id;
    }

    /**
     * Helper function that returns the spreadshit shop domain in use, either .com or .net. Useful for querying the Spreadshirt API.
     * @see SprdCMSItem
     *
     * @return string Returns com || net.
     */
    public function getDomain() {
        return (string) $this->xml->shop->domain;
    }

    /**
     * Helper method for querying the status of the URLs rewriting configuration.
     *
     * @see SprdCMSRewrite
     * @return boolean Will return TRUE or FALSE depending on configuration value.
     */
    public function getRewriteStatus() {
        return ((String) $this->xml->modrewrite === "1") ? TRUE : FALSE;
    }

    /**
     * When we use URL rewriting there is a risk that it will break relative paths.
     * So we provide the full path in order to construct links.
     *
     * @return  string Returns the base path in order to make relative links work.
     */
    public static function getBasePath() {
        self::$basePath .= "http://" . $_SERVER['SERVER_NAME'];
        self::$basePath .= preg_replace("/index.php$/", "", $_SERVER['PHP_SELF']);
        return self::$basePath;
    }

}

?>