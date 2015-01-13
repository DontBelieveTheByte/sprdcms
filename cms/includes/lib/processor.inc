<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * This interface guarantees that the process method will be implemented by different classes.
 *
 * This interface guarantees that the process method will be implemented by different classes.
 * It is mostly used for delegation to another object depending on the flow of the application.
 *
 * @package   SprdCMS
 */
interface SprdCMSProcessor {

    /**
     * Classes that implement this interface will need to implement this method.
     *
     * @param mixed $param Is kept NULL or can be overridden at runtime.
     */
    public function process($param = NULL);
}

