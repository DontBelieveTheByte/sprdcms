<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Central logging facility.
 *
 * Central logging facility.
 * It uses Exceptions and logs them to the `logs` database table.
 *
 * @package   SprdCMS
 */
class SprdCMSLogger {

    /**
     * Holds a reference to SprdCMSDb singleton isntance in order to make calls on the database.
     *
     * @see SprdCMSDb
     * @var SprdCMSDb
     */
    protected $database;

    /**
     * The constructor is only there to fill $sdb instance variable with a reference to a singleton database handle.
     */
    public function __construct() {
        $this->database = SprdCMSDb::getInstance();
    }

    /**
     * This method takes an exception and logs its message to the `logs` table.
     *
     * @param Exception $e We pass an exception and internally we call the Exception::getMessage to write a descriptive string of the exception event to the database.
     * @return void
     */
    public function logEvent(Exception $e) {
        $sql = "INSERT INTO `logs` (`type`, `message`) VALUES (:type, :message)";
        $bindings = array(
            ":type" => get_class($e),
            ":message" => $e->getMessage()
        );
        $this->database->prepare($sql, $bindings);
    }

    /**
     * This method is useful for the client to see what is actally inside the `logs` table.
     *
     * @return array|bool Returns an array of stdClass object reflecting logged Exceptions inside the `logs` table with latest logs at the beginning. If nothing is found it simply returns FALSE.
     */
    public function getLog() {
        $query = ("SELECT * FROM `logs` ORDER BY `time` DESC");
        return $this->database->prepare($query) ? $this->database->fetchAll() : FALSE;
    }

    /**
     * Sometimes the site administrator wants to clear the `logs` table after it has been reviewed, this is the method to use in that case.
     *
     * @return  bool It returns TRUE or FALSE depending on the success of clearing the `logs` table.
     */
    public function clearLog() {
        $sql = ("DELETE FROM `logs`");
        return $this->database->prepare($sql);
    }

}

