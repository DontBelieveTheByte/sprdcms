<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * This class is in charge of handling the caching mechanism.
 *
 * This class is in charge of handling the caching mechanism.
 * It decorates the SprdCMSCommandMapper class by first checking if there's a cache hit or a cache expiration.
 * Implementation of the SprdCMSProcessor interface garanties seamless delegation to SprdCMSCommandMapper
 * in order to  process the request when the cache doesn't exist or is expired.
 *
 * @package   SprdCMS
 * @todo Implement the class
 */
class SprdCMSCache implements SprdCMSProcessor {

    /**
     * Caching is not implemented yet.
     *
     * @todo    Implement the constructor method.
     */
    public function __construct() {
        ;
    }

    /**
     * Caching is not implemented yet.
     * @param array $name NULL to make SprdCMSPRocessor interface happy.
     * @todo Implement the constructor method.
     */
    public function process($param = NULL) {
        ;
    }

}

?>
