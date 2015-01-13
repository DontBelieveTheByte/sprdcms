<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */
require_once '../includes/lib/itemCSV.inc';
require_once '../includes/lib/itemManual.inc';

/**
 * This is base class that defines what properties a store items should hold and its common methods.
 *
 * This is base class that defines what properties a store items should hold and its common methods.
 * Instantiation of object is deffered via static factory methods that will return the correct type of child class object.
 *
 * @see SprdCMSItemCSV
 * @see SprdCMSItemManual
 *
 * @abstract
 * @package SprdCMS
 */
abstract class SprdCMSItem {

    /**
     * The unique ID of the article we want to in our store as an article, this is obtained from Spreadshirt.
     * @var string
     */
    protected $spreadShirtId;

    /**
     * The product ID of an article. Several articles can have the same product ID, this is obtained from Spreadshirt.
     * @var string
     */
    protected $spreadShirtProductId;

    /**
     * The name of of an article. This normally has semantic meaning to humans.
     * @var string
     */
    protected $name;

    /**
     * Articles can have a short textual description. This description can be updated by the site administrator.
     * @var string
     */
    protected $description;

    /**
     * Articles have two images mirrored on disk, this image is a zoom of the design being sold.
     * @var string
     */
    protected $image0;

    /**
     * Articles have two images mirrored on disk, this image shows the product with the included design.
     * @var string
     */
    protected $image1;

    /**
     * This normally holds the price of an item. It is set only once when the article is imported.
     * @var string
     */
    protected $price;

    /**
     * Until the Spreadshirt API is fully implement, we use an iframe to display a single article from Spreadshirt. This is the actual URL of an article from Spreadshirt.
     * @var string
     */
    protected $url;

    /**
     * The color of an article. Has semantic meaning to human but a hex value can be mapped later on.
     * @var string
     */
    protected $color;

    /**
     * Holds a reference to SprdCMSDb singleton instance in order to make calls on the database.
     *
     * @see SprdCMSDb
     * @var SprdCMSDb
     */
    protected $database;

    /**
     * The constructor's role is simply to obtain a database instance a set the reference to the $sdb property.
     *
     */
    public function __construct() {
        $this->database = SprdCMSDb::getInstance();
    }

    /**
     * This is a static factory method that should return an instance of SprdCMSItemCSV after calling its constructor.
     *
     * @static
     * @param array $item Takes an associative array built from the CSV file, json encoded/decoded and passes it to the SprdCMSItemCSV constructor.
     * @see SprdCMSItemCSV
     * @return SprdCMSItemCSV Returns an instance of a valid SprdCMSItemCSV object.
     */
    public static function SprdCMSitemCSVFactory($item) {
        return new SprdCMSItemCSV($item);
    }

    /**
     * This is a static factory method that should return a valid instance of SprdCMSitemManualFactory.
     *
     * @static
     * @param string $item Takes a string that represents a valid Spreadshirt article.
     * @see SprdCMSItemManual
     * @return SprdCMSItemManual Returns an instance of a valid SprdCMSItemManual object.
     * @todo The child class is not fully implemented yet, it will be ready for the full integration with the Spreadshirt API.
     */
    public static function SprdCMSitemManualFactory($item) {
        return new SprdCMSItemManual($item);
    }

    /**
     * Getter for the current instance's unique article ID field.
     *
     * @return  string The unique ID of the article.
     */
    public function getId() {
        return $this->spreadShirtId;
    }

    /**
     * Getter for the current instance's product ID. Several articles can refer to the same product ID.
     *
     * @return  string The unique ID of the product.
     */
    public function getProduct() {
        return (string) $this->spreadShirtProductId;
    }

    /**
     * Getter for the current instance's article name property.
     *
     * @return  string The human readable name of the articles beings sold. This can be null.
     */
    public function getName() {
        return $this->name;
    }

    /**
     * Getter for the current instance's image with zoom on the design, the img0 property.
     *
     * @return  string The file name on disk of the zoom image.
     */
    public function getImg0() {
        return $this->image0;
    }

    /**
     * Getter for the current instance's image showing the product with the design, the img1 property.
     *
     * @return  string The file name on disk of the article's image.
     */
    public function getImg1() {
        return $this->image1;
    }

    /**
     * Getter for the current instance's price with two decimals.
     *
     * @return  string The current price of the articles as it was first imported.
     */
    public function getPrice() {
        return $this->price;
    }

    /**
     * Getter for the current instance's description.
     *
     * @return  string A string of text representing a description.
     */
    public function getDescription() {
        return $this->description;
    }

    /**
     * Getter for the current instance's URL at Spreadshirt.
     *
     * @return  string The URL of an article being sold at Spreadshirt. This can be null in case of a fully integreted store with on site shopping cart.
     */
    public function getUrl() {
        return $this->url;
    }

    /**
     * Getter for the current instance's color.
     *
     * @return  string The human readable color.
     * @todo Map color name into their hex values.
     */
    public function getColor() {
        return $this->color;
    }

    /**
     * This method is in charge of saving a valid SprdCMSItem to the database.
     * It gets all the instance's field to build an associative array that will be passed to the database has a prepared SQL statement.
     *
     * @return  bool Returns TRUE on success of saving a new article or FALSE if there is a problem while doing so.
     * @throws  Exception Exceptions are logged to the `logs` table using the central logging facility and method returns FALSE if an Exception is triggered.
     * @todo Decouple this method into a persistance layer in a seperate class.
     */
    public function itemFreezeDb() {
        try {
            $bindings = array(':id' => $this->getId(),
                ':pid' => $this->getProduct(),
                ':name' => $this->getName(),
                ':desc' => $this->getDescription(),
                ':img0' => $this->getImg0(),
                ':img1' => $this->getImg1(),
                ':price' => $this->getPrice(),
                ':url' => $this->getUrl(),
                ':color' => $this->getColor()
            );
            $sql = "INSERT INTO `articles`(`id`, `pid`, `name`, `desc`, `img0`, `img1`, `price`, `url`, `color`) VALUES(:id, :pid, :name, :desc, :img0, :img1, :price, :url, :color)";
            if ($this->database->prepare($sql, $bindings) === TRUE) {
                return TRUE;
            }
            throw new Exception("Unable to save new item #" . $this->getId() . " database");
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

}

