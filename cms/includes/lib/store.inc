<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Abstract base class to define common admin and client store functionality.
 *
 * @author    J-F B <ever.livin.never.fearful@gmail.com>
 * @package   SprdCMS
 * @abstract
 */
abstract class SprdCMSStore {

    /**
     * Holds a reference to SprdCMSDb singleton isntance in order to make calls on the database.
     *
     * @see SprdCMSDb
     * @var SprdCMSDb
     */
    protected $sdb;

    /**
     * The constructor's role is simply to get a database instance from SprdCMSDb.
     * @see SprdCMSDb
     */
    function __construct() {
        $this->sdb = SprdCMSDb::getInstance();
    }

    /**
     * Abstact method that is later implemented to get all articles inside of a category.
     *
     * @param string A String representing the category we want to manipulate.
     */
    abstract public function getCategory($category);

    /**
     * Method used to get all the categories inside the store. Is only a specialized wrapped arount getSeries.
     *
     * @return array|boolean Returns an associative array of all the categories present inside the store or returns FALSE if no user defined category is set.
     */
    public function getCategories() {
        return $this->getSeries("categories");
    }

    /**
     * This is a helper method that queries the `sort` table and unserializes whatever is inside that particle row.
     *
     * @param string A string representing the name of the series we want to unserialize
     * @return array|boolean Returns an array reflecting the data structure of the serialized values or FALSE if there is no match.
     * @todo Decouple this methos from this class.
     */
    protected function getSeries($name) {
        $bindings = array(':name' => $name);
        if ($this->sdb->prepare("SELECT `info` FROM `sort` WHERE `sort_id` = :name", $bindings)) {
            $result = $this->sdb->fetch(PDO::FETCH_ASSOC);
            return isset($result['info']) ? unserialize($result['info']) : FALSE;
        } else {
            return FALSE;
        }
    }

    /**
     * Unfreeze an article from the database and inject it's values into a standard object.
     *
     * @param string A string representing the unique ID of a spreadshirt article.
     * @return stdClass|false Will return a standard class object representing article data or FALSE from the SprdCMSDb::fetch method.
     * @todo    Decouple this method from this class.
     */
    protected function itemThawDb($itemId) {
        require_once '../includes/lib/item.inc';
        $sql = "SELECT * FROM `articles` WHERE id = :id";
        $this->sdb->prepare($sql, array(":id" => $itemId));
        return $this->sdb->fetch();
    }

}

