<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * The install script.
 *
 * This class is instantiated only during installation of the application.
 * It takes care of setting up the environment needed to run SprdCMS.
 * There is no constructor method, we have to call ::initStore to attempt initialization.
 *
 * @package   SprdCMS
 */
class SprdCMSInstall {

    /**
     * The init method sequentially calls its internal methods and tests their return value.
     *
     * @return bool Once all the steps are completed and valid, it should return TRUE.
     * @throws Exception In the event of failure of any of the internal steps that return FALSE, we throw an exception and helper back to the user's browser.
     */
    public function initStore() {
        try {
            if ($this->filter() !== TRUE) {
                throw new Exception("Invalid form input. Server side validation fail.");
            }
            if ($this->filePerm() !== TRUE) {
                throw new Exception("Install failed on file permission. Make sure images and includes directory are both writable by the web server.");
            } elseif ($this->initXmlConfig() !== TRUE) {
                throw new Exception("Install failed on initializing XML config.");
            } elseif ($this->initDb() !== TRUE) {
                throw new Exception("Install failed on initializing database.");
            } elseif ($this->initPassword() !== TRUE) {
                throw new Exception("Install failed on intitializing password.");
            } else {
                return TRUE;
            }
        } catch (Exception $e) {
            $this->handleFailedInstall($e);
        }
    }

    private function handleFailedInstall($exception) {
        echo "<br>" . $exception->getMessage() . "<br>"; //handle exception here
        echo '<br>To reset the installation process, remove all tables from the database and delete includes/config.xml configuration file if present.<br>';
        exit();
    }

    /**
     * There is already user side form validation but we cross check server side using PHP's builting validation functions.
     *
     * @return  bool Returns TRUE or FALSE reflecting the outcome of validating the user's input.
     */
    private function filter() {
        $filters = array(
            'shopName' => array(
                "filter" => FILTER_VALIDATE_REGEXP,
                "options" => array(
                    "regexp" => "/^.{2,50}$/"
                )
            ),
            'shopId' => array(
                "filter" => FILTER_VALIDATE_REGEXP,
                "options" => array(
                    "regexp" => "/^[0-9]{6,6}$/"
                )
            ),
            'domain' => array(
                "filter" => FILTER_VALIDATE_REGEXP,
                "options" => array(
                    "regexp" => "/[com|net]/"
                )
            ),
            'username' => array(
                "filter" => FILTER_VALIDATE_REGEXP,
                "options" => array(
                    "regexp" => "/^.{3,35}$/"
                )
            ),
            'password' => array(
                "filter" => FILTER_VALIDATE_REGEXP,
                "options" => array(
                    "regexp" => "/^.{8,50}$/"
                )
            ),
            'dbName' => array(
                "filter" => FILTER_CALLBACK,
                "options" => "is_string"
            ),
            'dbUser' => array(
                "filter" => FILTER_CALLBACK,
                "options" => "is_string"
            ),
            'dbPass' => array(
                "filter" => FILTER_CALLBACK,
                "options" => "is_string"
            ),
            'dbHost' => array(
                "filter" => FILTER_CALLBACK,
                "options" => "is_string"
            ),
            'dbPort' => array(
                "filter" => FILTER_VALIDATE_REGEXP,
                "options" => array(
                    "regexp" => "/^[0-9]{2,6}$/"
                )
            ),
            'email' => FILTER_VALIDATE_EMAIL);
        $this->post = filter_input_array(INPUT_POST, $filters);
        foreach ($this->post as $post) {
            if (empty($post)) {
                return FALSE;
            }
        }
        return TRUE;
    }

    /**
     * SpdCMS needs an application wide configuration file in order to run.
     * @see SprdCMSSiteConfig
     * This method is only called after the fact that the input is validated, so we know $_POST contains sane values.
     * A string is built in order to write an XML file is to disk.
     *
     * @return  bool Returns TRUE or FALSE matching the outcome of writing the content of the XML configuration file to the disk.
     */
    private function initXmlConfig() {
        $xmlConfigPayload =
                '<?xml version="1.0" encoding="UTF-8"?>' .
                '<config>' .
                '<db>' .
                '<name>' . $_POST['dbName'] . '</name>' .
                '<host>' . $_POST['dbHost'] . '</host>' .
                '<port>' . $_POST['dbPort'] . '</port>' .
                '<username>' . $_POST['dbUser'] . '</username>' .
                '<password>' . $_POST['dbPass'] . '</password>' .
                '</db>' .
                '<shop>' .
                '<store>' . $_POST['shopName'] . '</store>' .
                '<id>' . $_POST['shopId'] . '</id>' .
                '<domain>' . $_POST['domain'] . '</domain>' .
                '</shop>' .
                '<modrewrite>0</modrewrite>' .
                '</config>';
        return (@file_put_contents("includes/config.xml", $xmlConfigPayload) !== FALSE) ? TRUE : FALSE;
    }

    /**
     * This is where the database schema is defined and tables are initialized.
     * The method can only be called after the configuration file has been written since we use the SprdCMSDb class.
     * @see SprdCMSDb
     * @throws Exception Failure to create table will trigger exceptions and also return FALSE.
     * @return  bool Returns TRUE if all tables in the schema are created correctly or FALSE if there is any errors.
     */
    private function initDb() {

        $sdb = SprdCMSDb::getInstance();
        $tables = array(
            'articles' => "CREATE TABLE IF NOT EXISTS `articles`
                                (`id` char(8) PRIMARY KEY,
                                `pid` char(8) NOT NULL,
                                `name` varchar(255) NOT NULL,
                                `desc` text NULL,
                                `color` varchar(255) NULL,
                                `img0` varchar(255) NOT NULL,
                                `img1` varchar(255) NOT NULL,
                                `price` decimal(5,2) NOT NULL,
                                `url` text NOT NULL) ENGINE=InnoDB",
            'login' => "CREATE TABLE IF NOT EXISTS `login`
                                (`name` varchar(255) PRIMARY KEY,
                                `password` varchar(255) NOT NULL) ENGINE=InnoDB",
            'category' => "CREATE TABLE IF NOT EXISTS `categories`
                                (`article_id` varchar(256) PRIMARY KEY,
                                `Home` tinyint(1) NULL) ENGINE=InnoDB",
            'sort' => "CREATE TABLE IF NOT EXISTS `sort`
                                (`sort_id` varchar(256) PRIMARY KEY,
                                `info` mediumtext NULL) ENGINE=InnoDB",
            'cache' => "CREATE TABLE IF NOT EXISTS `cache`
                                (`category_id` varchar(256) PRIMARY KEY,
                                `info` mediumtext NULL) ENGINE=InnoDB",
            'logs' => "CREATE TABLE IF NOT EXISTS `logs`
                                (`type` varchar(256) NULL,
                                 `time` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                                 `message` mediumtext NULL) ENGINE=InnoDB"
        );
        foreach ($tables as $table) {
            if ($sdb->prepare($table) === FALSE) {
                throw new Exception("Error initializing $table <br>");
                return FALSE;
            }
        }
        $sort_id = array("categories", "Home");
        foreach ($sort_id as $value) {
            $bindings = array(
                ':sort_id' => $value
            );
            if ($sdb->prepare("INSERT INTO `sort`(`sort_id`) VALUES (:sort_id)", $bindings) === FALSE) {
                throw new Exception("Error inserting into sort table. <br>");
                return FALSE;
            }
        }
        if ($sdb->prepare("INSERT INTO `cache`(`category_id`) VALUES ('Home')") === FALSE) {
            throw new Exception("Error inserting into caching table. <br>");
            return FALSE;
        }
        return TRUE;
    }

    /**
     * There are two directories that need to be writable by SprdCMS.
     * includes needs to be writable to save and modify the configuration file.
     * images needs to be writable in order to cache the article's images on disk.
     *
     * @return bool If any of the directories are not writable by the application it will return FALSE otherwise returns TRUE.
     */
    private function filePerm() {
        clearstatcache(); // if we do multiple check operations caching can return false data
        if (!is_writable("./images/") || !is_writable("./includes/")) {
            return FALSE;
        }
        return TRUE;
    }

    /**
     * The sets the site administrator's username and password for logging in.
     * The salt used is a simple hash of the database name.
     * No salt algorithm is proposed for now and hashing method is left to the current default PHP installation configuration of the crypt function.
     *
     * @return  bool TRUE is returned if the crypted value is written to the database or FALSE if cannot save it.
     * @todo test for crypt constants in order to use the best hash available on the current PHP installation.
     */
    private function initPassword() {
        $sdb = SprdCMSDb::getInstance();
        $crypted = crypt($_POST['password'], md5($_POST['dbName']));
        $params = array(
            ':name' => $_POST['username'],
            ':password' => $crypted
        );
        return $sdb->prepare("INSERT INTO login (`name`,`password`) VALUES (:name, :password)", $params);
    }

}
