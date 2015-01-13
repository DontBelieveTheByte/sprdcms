
<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */
require_once '../includes/lib/store.inc';

/**
 * Class is responsible for administering the SprdCMS backend.
 *
 * The backstore extends the abstract SprdCMSBackStore class.
 * It is responsible for administering the SprdCMS backed by adding, sorting and deleting articles and categories.
 *
 * @see SprdCMSBackStore
 * @package   SprdCMS
 */
class SprdCMSBackStore extends SprdCMSStore {

    /**
     * The constructor role is just to call the parent's constructor.
     *
     * @see SprdCMSStore
     */
    function __construct() {
        parent::__construct();
    }

    /**
     * The method returns all the article ID that are present in the store.
     * It is useful for the view using Javascript to signal to the user when an article is already present.
     *
     * @return mixed Will Return an array of all unique article IDs or boolean FALSE if empty.
     */
    public function getAllIds() {
        try {
            $this->sdb->prepare("SELECT `id` FROM articles");
            return $result['id'] = $this->sdb->fetchAll();
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE; //Return false on failure by default after Db logging.
        }
    }

    /**
     * Instantiate an Item with manual input using an article ID.
     *
     * @see SprdCMSItemManual
     * @param string $item The unique item ID of the new item.
     * @todo Implement the method when spreadshirt API is added.
     */
    public function addShopItemManual($item) { // not implimented in lite version
        //TODO
    }

    /**
     * Instantiate a SprdCMSItemCSV object from the input of a CSV file.
     *
     * @see SprdCMSItemCSV
     *
     * @param array $item Accepts an array of CSV fields.
     * @return boolean Returns TRUE or FALSE depending on outcome of the adding a new article operation.
     * @throws Exception Logs failure of attemps to add an article to the database and returns FALSE.
     */
    public function addShopItemCSV($item) {
        try {
            require_once '../includes/lib/item.inc';
            $newItem = SprdCMSItem::SprdCMSitemCSVFactory($item);
            if ($newItem->getImages() === TRUE && $newItem->itemFreezeDb() === TRUE) {
                $bindings = array(':article_id' => $newItem->getId());
                if ($this->sdb->prepare("INSERT INTO `categories` (`article_id`) VALUES (:article_id)", $bindings) === TRUE) {//Add item to the articles table matrix.
                    $categories = array_reverse($this->getCategories()); //Little array gymnastic to be Home in front.
                    $categories["Home"] = "Home";
                    foreach (array_reverse($categories) as $machineName => $category) { //Append new item to all the individual present categories.
                        $tmpArr = $this->getSeries($machineName);
                        $tmpArr[] = $newItem->getId(); //Append the new item to the array of that particular category.
                        if ($this->saveSeries($machineName, $tmpArr) === FALSE) {
                            throw new Exception("Error adding " . $item->getId() . "to the " . $category . " category.");
                        }
                    }
                    return TRUE;
                }
            }
            throw new Exception("Error adding item : " . $item->getId() . " to the store.");
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE; //Return false on failure by default after Db logging.
        }
    }

    /**
     * Remove an item from the SprdCMS store.
     *
     * @param string $itemId Accepts a string matching a unique identifier and deletes the item from the store.
     * @return boolean Returns TRUE on success and FALSE on failure.
     * @throws Exception Logs exceptions and relevant messages to the `logs` database table and returns FALSE.
     */
    public function delShopItem($itemId) {
        try {
            $categories = array_reverse($this->getCategories()); //Little array gymnastic to be Home in front.
            $categories["Home"] = "Home";
            foreach (array_reverse($categories) as $machineName => $category) { //Append new item to all the individual present categories.
                $tmpArr = array_diff($this->getSeries($machineName), array($itemId));
                if ($this->saveSeries($machineName, $tmpArr) === FALSE) {
                    throw new Exception("Problem while deleting item : $itemId from store. Unable to substract and save result to the category $category");
                }
            }
            $bindings = array(':item' => $itemId);
            return $this->sdb->prepare("DELETE FROM `categories` WHERE `article_id` = :item", $bindings) && //multi line
                    $this->sdb->prepare("DELETE FROM `articles` WHERE `id` = :item", $bindings);
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Method used to toggle On or Off the viewing of an article for site visitors.
     *
     * @param array $params Index[0] is a unique identifier matching category we wabt to change and index[1] is the ID of the article we want to toggle On or Off.
     * @return boolean Returns TRUE or FALSE depending on the outcome of the operation.
     * @throws Exception Logs exceptions and relevant messages to the `logs` database table and returns FALSE.
     */
    public function activeToggleItem(Array $params) {
        try {
            $sql = "UPDATE `categories` SET `$params[0]` = IF( `$params[0]` = 1, 0, 1) WHERE `article_id` = :article_id";
            $bindings = array(
                ':article_id' => $params[1]
            );
            return $this->sdb->prepare($sql, $bindings);
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Update an article's description.
     *
     * @param Array $params The item id at array index[0] and the updated description at array index[1].
     * @return boolean Returns TRUE or FALSE depending on the outcome of the operation.
     * @throws Exception Logs exceptions and relevant messages to the `logs` database table and returns FALSE.
     */
    public function updateItemDesc(Array $params) {
        try {
            $binding = array(
                ':id' => $params[0],
                ':desc' => $params[1]
            );
            return $this->sdb->prepare("UPDATE `articles` SET `desc` = :desc WHERE `id` = :id", $binding);
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Method used to enter a new category. The new category will be displayed on the menu of SprdCMS store front.
     * Input is checked for duplication an sanitized using regular expressions.
     * @param string $name The name of a new category.
     * @return boolean Return TRUE or FALSE depending on outcome of the operation of adding a new category.
     * @throws Exception Logs exceptions and relevant messages to the `logs` database table and returns FALSE.
     * @todo More elegant regex.
     */
    public function setCategory($name) {
        $machineName = preg_replace('/|[^a-zA-Z0-9_ -]/s', '', $name); //Strip anything but letters, numbers, underscore, hyphen and space.
        $machineName = preg_replace('/[ -]/s', '_', $machineName); //Turn other seperators into underscore.
        $machineName = preg_replace('/^[0-9_]/', '_', $machineName); //Valid MySql identifiers can't start with a number or underscore for Javascript URL encoding.
        $machineName = preg_replace('/[_]+/', '_', $machineName); //Substiture repeating underscores with a single one.
        try {
            $this->sdb->beginTransaction();
            $categories = $this->getSeries("categories");
            if ($categories) {//We need this error checking for when there are no categories defined yet.
                while (array_key_exists($machineName, $categories)) {//Prevent getting 2 categories with the same machine name.
                    $machineName .= "_";
                }
            }
            $bindings = array(':sort_id' => $machineName);
            $this->sdb->prepare("INSERT INTO `sort`(`sort_id`) VALUES (:sort_id)", $bindings);
            $tempArr = $this->getSeries("categories");
            $tempArr[$machineName] = "$name";
            $this->saveSeries("categories", $tempArr); //Add the new category to the serialized categories.
            $this->saveSeries($machineName, $this->getSeries("Home")); //Use Home as a template for the new category.
            $sql = ("ALTER TABLE `categories` ADD " . $machineName . " tinyint(1) NULL"); //Add to the category matrix. This also does an implicit commit since we alter a table.
            $this->sdb->exec($sql);
            return $this->getCategories();
        } catch (Exception $exc) {
            $this->sdb->rollBack();
            $logger = new SprdCMSLogger();
            $logger->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Delete a category from the store. All articles remain but the category context is lost.
     *
     * @param string $name The machine name of the category to be deleted.
     * @return boolean Returns TRUE or FALSE depending on the success of deleting a category.
     * @throws Exception Logs exceptions and relevant messages to the `logs` database table and returns FALSE.
     */
    public function delCategory($name) {
        try {
            $this->sdb->beginTransaction();
            $bindings = array(':name' => $name);
            if ($this->sdb->prepare("DELETE FROM `sort` WHERE `sort_id` = :name", $bindings) === TRUE) {//Delete serialized articles row.
                $filter = array(); //[] Can't use this syntax with PHP 5.2
                $categories = $this->getSeries("Categories");
                foreach ($this->getSeries("Categories") as $key => $value) {//Rebuild the categories list.
                    if ($key !== $name) {
                        $filter[$key] = $categories[$key];
                    }
                }
                if ($this->saveSeries("Categories", $filter) === TRUE) {//Save the new categories list.
                    return ($this->sdb->exec("ALTER TABLE `categories` DROP $name") !== FALSE) ? TRUE : FALSE; //This does an implicit commit since we alter a table.
                }
            }
            throw new Exception("Problem while deleting category : $name"); //Any failed if statements will trigger an Exception.
        } catch (Exception $exc) {
            $this->sdb->rollBack();
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Get a category with all of its relevant ordered articles.
     *
     * @param string $category The machine name of the category.
     * @return mixed Will return boolean FALSE on failure, otherwise returns an array of stdClass objects.
     */
    public function getCategory($category) {
        $itemList = $this->getSeries($category); //unserialize the requested category
        if ($itemList !== FALSE) {
            $response = array();
            foreach ($itemList as $itemId) {
                $sql = "SELECT articles.*, categories." . $category . " FROM `articles` INNER JOIN  `categories` ON articles.id = categories.article_id  WHERE id = :item_id";
                $bindings = array(
                    ':item_id' => $itemId
                );
                $this->sdb->prepare($sql, $bindings);
                $result = $this->sdb->fetch();
                $result->active = $result->{$category};
                unset($result->{$category});
                $response[] = $result;
            }
            return $response;
        }
        return FALSE;
    }

    /**
     * Handle logs from client side requests.
     *
     * @param string $command Should be either getLog or clearLog.
     * @return bool Returns TRUE or FALSE depending on the outcome of the requested commande.
     * @throws Exception Logs exceptions on invalid commands and it's relevant message to the `logs` database table and returns FALSE.
     * @todo Decouple this method into it's own class.
     */
    public function logs($command) {
        try {
            if ($command === "getLogs") {
                $log = new SprdCMSLogger();
                return $log->getLog();
            } elseif ($command === "clearLogs") {
                $log = new SprdCMSLogger();
                return $log->clearLog();
            } else {
                throw new Exception("Invalid command on the logger remote interface");
            }
        } catch (Exception $exp) {
            $log = new SprdCMSLogger();
            $log->logEvent($exp);
            return FALSE;
        }
    }

    /**
     * Takes care of sorting articles inside of a single category.
     *
     * @param array $array An array containing ['category'] as the category and ['param'] with another array of all the ID in numerical order.
     * @return boolean Returns TRUE or FALSE and logs Exceptions before FALSE.
     * @throws Exception Logs exceptions to the `logs` table on erratic behavior and returns boolean FALSE.
     */
    public function sortCategory(Array $array) {
        try {
            $this->sdb->beginTransaction();
            if (array_key_exists($array["category"], $this->getSeries("categories")) || $array["category"] === "Home") { //Check if category exists or default Home.
                if ($this->saveSeries($array["category"], $array["info"]) === TRUE) {
                    $this->sdb->commit();
                    return TRUE;
                }
            }
            throw new Exception("Error while attempting to sort category : " . $array['category'] . ".");
        } catch (Exception $exc) {
            $this->sdb->rollBack();
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Method to sort the list of categories according to the administrative user defined preference.
     *
     * @param array $array An array of sorted categories machine names in numerical order.
     * @return boolean Returns TRUE or FALSE and logs Exceptions before FALSE.
     * @throws Exception Erratic behavior is logged to the `logs` table and boolean FALSE is returned.
     */
    public function sortCategories(Array $array) {
        try {
            $categories = $this->getSeries("Categories");
            $newCategories = array(); //[] Does not work with PHP 5.2
            if ($categories) {
                foreach ($array as $key => $value) {
                    if (!empty($categories[$value])) { //Some basic error checking to see if the category even exists.
                        $newCategories[$value] = $categories[$value];
                    }
                }
                return $this->saveSeries("Categories", $newCategories);
            }
            throw new Exception("No categories to sort.");
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Private method to serialize data and save it to the `sort` table for persistance.
     *
     * @param string $name The name of the series we want to save.
     * @param array $value An array that can be multidimensional or flat, it just handle persistance of data structures.
     * @return bool Returns TRUE on success or FALSE on failure to save the serialized data.
     * @throws Exception Erratic behavior is logged to the `logs` table and boolean FALSE is returned.
     * @todo Decouple this method into a persistance layer.
     */
    private function saveSeries($name, $value) {
        try {
            $bindings = array(
                ':name' => $name,
                ':info' => serialize($value)
            );
            if ($this->sdb->prepare("UPDATE `sort` SET `info` = :info WHERE `sort_id` = :name", $bindings) === TRUE) {
                return TRUE;
            } else {
                throw new Exception("Unable to save series $name");
            }
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

}

