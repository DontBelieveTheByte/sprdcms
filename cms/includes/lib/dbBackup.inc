<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Provides the functionality of backing up the MySQL database.
 *
 * Provides the functionality of backing up the MySQL database.
 * It will only work where the MySQL database runs on the same host as the webserver and PHP has shell access.
 * Implements SprdCMSProcessor interface when the request is determined to be delegated to this object.
 * @see SprdCMSProcessor
 *
 * @package   SprdCMS
 */
class SprdCMSDbBackup implements SprdCMSProcessor {

    /**
     * Holds a reference to an instance of SprdCMSSiteConfig
     * @var SprdCMSSiteConfig
     */
    private $config;

    /**
     * The constructor gets an instance of the site wide configuration needed to build the backup command.
     *
     * @return  void
     */
    public function __construct() {
        $this->config = SprdCMSSiteConfig::getConfig();
    }

    /**
     * The delegation methods builds the backup command using the configuration file.
     * It saves a database dump to disk and sends it back to the user's browser a a downloadable file.
     * This is achieved by handling the request directly and settings the http protocol header.
     * @return void
     * @throws Exception Failure to backup the MySQL database will be logged. The user has to check the logs to see if something is wrong with the backup process.
     * @param mixed $param = NULL to keep the interface happy.
     * @todo Check to make sure the username isn't already taken
     */
    public function process($param = NULL) {
        try {
            $command = 'mysqldump --single-transaction -u' . $this->config->getDbUser() . ' -p' . $this->config->getDbPass() . ' --databases ' . $this->config->getDbName() . ' > includes/SprdCMSSQLdump.sql';
            exec($command);
            if (@file_exists('includes/SprdCMSSQLdump.sql')) {
                $size = filesize('includes/SprdCMSSQLdump.sql');
                header('Content-Description: File Transfer');
                header('Content-Type: application/octet-stream');
                header('Content-Disposition: attachment; filename=SprdCMSSQLdump.sql');
                header('Content-Transfer-Encoding: binary');
                header('Expires: 0');
                header('Cache-Control: must-revalidate');
                header('Pragma: public');
                header('Content-Length: ' . $size);
                ob_clean();
                flush();
                readfile('includes/SprdCMSSQLdump.sql');
                exit;
            } else {
                throw new Exception("Unable to backup SQL database.");
            }
        } catch (Exception $e) {
            $log = new SprdCMSLogger();
            $log->logEvent($e);
            exit();
        }
    }

}

