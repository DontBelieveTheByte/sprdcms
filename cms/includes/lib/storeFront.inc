<?php

/**
 * @copyright (c) 2013, J-F B
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 */
require_once '../includes/lib/store.inc';

/**
 * Provides client side store functionality by extending SprdCMSStore base class.
 *
 * Provides client side store functionality by extending SprdCMSStore base class.
 * The constructor will construct the parent class and initialize common values regardless of the actual request.
 *
 * @package   SprdCMS
 */
class SprdCMSStoreFront extends SprdCMSStore {

    /**
     * An associative array with each keys representing an element the view will later need to render.
     * @var array
     */
    private $renderable = array(); //Can't use [] for implicit array declaration below PHP 5.3

    /**
     * Construct the parent first and assign keys and values that are common to all request.
     * Most of the values are obtained from the central configuration except the list of categories.
     *
     * @return void
     */

    public function __construct() {
        parent::__construct();
        $this->renderable['basePath'] = SprdCMSSiteConfig::getBasePath();
        $this->renderable['modRewrite'] = SprdCMSSiteConfig::getConfig()->getRewriteStatus();
        $this->renderable['store'] = SprdCMSSiteConfig::getConfig()->getStoreName();
        $this->renderable['shopId'] = SprdCMSSiteConfig::getConfig()->getShopID();
        $this->renderable['shopDomain'] = SprdCMSSiteConfig::getConfig()->getDomain();
        $this->renderable['catList'] = $this->getCategories();
    }

    /**
     * Methos that is used to get a renderable array in a single store article context.
     *
     * @param string $article The unique ID of an article as given by Spreadshirt.
     * @return mixed Returns an associative array or boolean FALSE if the method can't find an article matching the $article parameter.
     * @throws  Exception Failed attempt to load an article (probably because it doesn't exist) will be logged along with the ID of that article and return false.
     */
    public function getArticle($article) {
        try {
            $this->renderable['article'] = $this->itemThawDb($article);
            if (!$this->renderable['article']) {
                throw new Exception("The resquested article '$article' doesn't seem to exist.");
            }
            $this->renderable['name'] = $this->renderable['article']->name;
            return $this->renderable;
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Sets the username for the given user instance. If the username
     * is already set, it will be overwritten. Throws an invalid
     * argument exception if the provided username is of an invalid
     * format.
     *
     * @param string $category A String representing the name of the category we're looking to render.
     * @return mixed Returns Boolean FALSE on errors or an associate array with all the keys used to list a full category.
     * @throws  Exception unknown categories request are logged and the method will return FALSE
     */
    public function getCategory($category) {
        try {
            $this->renderable['name'] = $this->renderable['catList'][$category];
            if (!$this->renderable['name']) {
                throw new Exception("The requested category '$category' doesn't seem to exist.");
            }
            $itemList = $this->getSeries($category); //unserialize the requested category into an array
            if ($itemList) {
                foreach ($itemList as $itemId) {
                    $sql = "SELECT articles.*, categories.$category FROM `articles` INNER JOIN  `categories` ON articles.id = categories.article_id  WHERE id = :item_id";
                    $bindings = array(
                        ':item_id' => $itemId
                    );
                    $this->sdb->prepare($sql, $bindings);
                    $result = $this->sdb->fetch();
                    if ($result->{$category} === "1") {
                        $this->renderable['articles'][] = $result;
                    }
                }
            }
            return $this->renderable;
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

    /**
     * Builds a list of all the categories along with the Home category by default.
     * The keys of the associative array represents the machine name and database column
     * for that particular category while the values represents the human readable name as set by the user.
     *
     * @return array An associative array of all categories present inside the store.
     */
    public function getCategories() {
        $tmpList = parent::getCategories();
        if (!empty($tmpList)) {
            $sort = array_reverse($tmpList);
            $sort["Home"] = "Home";
            return array_reverse($sort);
        } else {
            return array("Home" => "Home");
        }
    }

}

