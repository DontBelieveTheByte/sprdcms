<?php

//error_reporting(0);

/**
 * @package SprdCMS
 * @copyright (c) 2013, J-F B
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 */
require_once './includes/lib/processor.inc';
require_once './includes/lib/frontController.inc';
require_once './includes/lib/request.inc';
require_once './includes/lib/commandMapper.inc';
require_once './includes/lib/viewController.inc';
require_once './includes/lib/rewrite.inc';
require_once './includes/lib/siteConfig.inc';
require_once './includes/lib/db.inc';
require_once './includes/lib/logger.inc';
try {
    $controller = new SprdCMSFrontController(
        new SprdCMSRequest(),
        new SprdCMSCommandMapper(new SprdCMSViewControl())
    );
    $controller->process();
} catch (Exception $exc) { //Catch any uncaught exception.
    $log = new SprdCMSLogger();
    $log->logEvent($exc);
    echo 'Top level application error.';
    exit();
}

