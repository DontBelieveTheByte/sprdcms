<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * A PDO wrapper class used for handling database operations that is implemented as a singleton.
 *
 * A PDO wrapper class used for handling database operations that is implemented as a singleton.
 * The class uses an application wide configuration to build a data source name (DSN)
 * and initialize a PDO instance.
 * The constructor is private and can only be called via the static method getInstance.
 * getInstance returns the same database handle throughout the application.
 *
 * @see SprdCMSSiteConfig
 * @package   SprdCMS
 */
Class SprdCMSDb {

    /**
     * A reference to an instance of a site wide configuration.
     * @var SprdCMSSiteConfig
     */
    private $config;

    /**
     * An instance of a PDO database handle.
     * @var PDO
     */
    private $dbh;

    /**
     * An array with database connection information used to build the DSN (data source name) needed to construct a PDO object.
     * @var array
     */
    private $connectStr;

    /**
     * A reference to a PDOStatement object.
     * @var PDOStatement
     */
    private $stm;

    /**
     * This class variable holds a refenrence to a single PDO instance.
     * @var PDO
     */
    private static $instance;

    /**
     * The constructor is prive and can only be called by the static method getInstance.
     * It sets a single instance that is held inside of a static private variable.
     */
    private function __construct() {
        $this->config = SprdCMSSiteConfig::getConfig();
        $this->connectStr = $this->config->getdbCredentials();
        $this->dbh = new PDO($this->connectStr[0], $this->connectStr[1], $this->connectStr[2]);
        $this->dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $this->dbh->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_OBJ);
    }

    /**
     * Get a database instance. This is the public interface where we must go through in order to acquire a database handle.
     *
     * @static
     * @return PDO Returns a single PDO instance no matter how many times it is called.
     */
    public static function getInstance() {
        if (!isset(self::$instance)) {
            self::$instance = new SprdCMSDb();
        }
        return self::$instance;
    }

    /**
     * PDO::exec wrapper for SQL statements. PDO::exec does not sanitize any values.
     *
     * @param string $sql Takes a SQL statement as string and delegates to PDO::exec.
     * @return int|bool Returns an integer representing the number of rows affected so 0 is not === FALSE in case we alter a table. Returns boolean FALSE on failure.
     */
    public function exec($sql) {
        return $this->dbh->exec($sql);
    }

    /**
     * Simple wrapper around PDO::beginTransaction.
     *
     * @return  bool Note that this still returns TRUE when transaction are not supported by the database backend (when not using MyIsam).
     */
    public function beginTransaction() {
        return $this->dbh->beginTransaction();
    }

    /**
     * Simple wrapper around PDO::commit.
     *
     * @return  bool Note that this still returns TRUE when transaction are not supported by the database backend (when not using MyIsam).
     */
    public function commit() {
        return $this->dbh->commit();
    }

    /**
     * Simple wrapper around PDO::rollBack.
     *
     * @return  bool Note that this still returns TRUE when transaction are not supported by the database backend (when not using MyIsam).
     */
    public function rollBack() {
        return $this->dbh->rollBack();
    }

    /**
     * Run an SQL query using PDO::query.
     *
     * @param string A valid SQL query, input is not sanitized and should not be used in most cases.
     * @return PDOStatement|bool Returns a PDOStatement to fetch the values or boolean FALSE depending on validity.
     */
    public function query($sql) {
        return $this->dbh->query($sql);
    }

    /**
     * Run prepared SQL statements with PDO::prepare.
     *
     * @param string The string containing the SQL command with valid syntax.
     * @param array Defaults to NULL or an associative array of binded values expected by the prepared statement. Dangerous values of unsanitized input should be escaped during this call.
     * @return boolean Method returns TRUE or FALSE depending on outcome of the PDO::execute method because it is automatically called.
     */
    public function prepare($sql, Array $params = NULL) {
        $this->stm = $this->dbh->prepare($sql);
        if ($params != NULL) {
            foreach ($params as $key => $value) {
                $this->stm->bindValue($key, $value);
            }
        }
        return $this->stm->execute();
    }

    /**
     * Fetch everything at once from a prepared statement using the PDO::fetchALL method.
     *
     * @param string Should be a PDO::CONSTANT, we can specify what type of data we get back, default is a stdClass object if nothing is specified.
     * @return mixed Return is specified the the $fetchType parameter and must be a PDO constant. Default value is PDO::FETCH_OBJ.
     */
    public function fetchAll($fetchType = NULL) {
        return $fetchType ? $this->stm->fetch($fetchType) : $this->stm->fetchAll();
    }

    /**
     * Fetch one row at the time from a prepared statement.
     *
     * @param string Should be a PDO::CONSTANT. Default value is PDO::FETCH_OBJ or otherwise specified parameter.
     * @return mixed Return is specified the the $fetchType parameter and must be a PDO constant. Default value is PDO::FETCH_OBJ.
     */
    public function fetch($fetchType = NULL) {
        return $fetchType ? $this->stm->fetch($fetchType) : $this->stm->fetch();
    }

    /**
     * Wrapper arount PDO::quote, normally used outside the context of a PDO prepared statement.
     *
     * @param string The string we want to quote.
     * @return string The quoted string.
     */
    public function quote($string) {
        return $this->dbh->quote($string);
    }

}

?>