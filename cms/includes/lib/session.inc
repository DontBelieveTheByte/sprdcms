<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Simple class used to control session of the site administator.
 *
 * The constructor of the User class takes a Data Mapper which
 * handles the user persistence in the database. In this case, 
 * we will provide a fake one.
 *
 * @package   SprdCMS
 * @author    J-F B <ever.livin.never.fearful@gmail.com>
 */
class SprdCMSSession {

    /**
     * Holds a reference to SprdCMSDb singleton isntance in order to make calls on the database. This property is static because class in never instantiated.
     *
     * @static
     * @var SprdCMSDb
     */
    protected static $sdb;

    /**
     * Safely starts a session. We use this wrapper to make sure no errors arise when a session is started more than once.
     *
     * @static
     * @return  void
     */
    public static function start() {
        if (!isset($_SESSION)) {
            session_start();
        }
    }

    /**
     * Simply destroys site administrator $_SESSION. This had the effect of logggin him/her out.
     *
     * @static
     * @return  void
     */
    public static function stop() {
        self::start();
        session_destroy();
    }

    /**
     * Validates the input parameters and compared the username of the site administrator along with the hashed password.
     * Sets the $_SESSION['loggedIn'] superglobal only if login passed.
     *
     * @static
     * @param string $name The username obtained from the login form input.
     * @param string $password The raw password coming from the login form input.
     * @return  void
     */
    public static function adminLogin($name, $password) {
        self::start();
        self::$sdb = SprdCMSDb::getInstance();
        $config = SprdCMSSiteConfig::getConfig();
        $values = array(
            ":name" => "$name",
            ":password" => crypt($password, md5($config->getDbName()))
        );
        self::$sdb->prepare("SELECT COUNT(*) FROM `login` WHERE `name` = :name AND `password`= :password", $values);
        $outcome = self::$sdb->fetch(PDO::FETCH_NUM);
        if ($outcome[0] == 1) {
            $_SESSION['loggedIn'] = "checked";
        }
    }

    /**
     * Helper function to verify the logged status of an administrator making a request. Session::start is called automatically.
     *
     * @return bool Returns TRUE or FALSE depending if the site administrator is already authenticated or not.
     */
    public static function getLoginStatus() {
        self::start();
        return isset($_SESSION['loggedIn']) && $_SESSION['loggedIn'] == "checked";
    }

}

