<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * Class useful for instantiating store articles built from a CSV file.
 *
 * This class extends SprdCMSItem.
 * @see SprdCMSItem
 * It specializes into holding an instance of an item build from the input of a CSV file.

 * @package   SprdCMS
 */
class SprdCMSItemCSV extends SprdCMSItem {

    /**
     * An instance of the application's configuration registry.
     * @var SprdCMSSiteConfig
     */
    protected $config;

    /**
     * The constructor starts by calling the parent's constructor.
     * @see SprdCMSItem
     * It also gets an instance of the application wide configuration registry and sets the reference to the $config property.
     * @see SprdCMSSiteConfig
     * The constructor is usually called within the context of a static factory method of the abstract parent class.
     * It uses an associative array as its input parameters for setting the properties of the instance.
     * Some properties can be NULL while other cannot, see the database schema defined in SprdCMSInstall::initDb.
     * @see SprdCMSInstall::initDb
     *
     * @param array $item A numerically index array matching all the necessary CSV fields as input.
     */
    public function __construct(Array $item) {
        parent::__construct();
        $this->config = SprdCMSSiteConfig::getConfig();
        $this->id = $item[0];
        $this->pid = $item[1];
        $this->name = $item[2];
        $this->desc = $item[3];
        $this->color = $item[6];
        $this->price = $item[7];
        $this->url = $item[10];
    }

    /**
     * This method reflects the idiosyncratic way to obtan an image from CSV input.
     * Instead of keeping a reference URLs to the images on spreadshirt servers, we cache them onto the site servers.
     * This is better for two reasons:
     * 1. Faster access that can also be offloaded to a CDN.
     * 2. Images file names have semantic meaning which is better for humans and search engine indexation.
     * The img0 and img1 properties are set to the file names of cached data on disk and we these values to build the visitor's site instead.
     *
     * @return  bool Method returns TRUE when saving the images succeeds or FALSE after an exception is triggered by an error in the flow of the method.
     * @throws  Exception Exceptions are logged through the central logging facility with the use of the `logs` table and FALSE is finally returned.
     */
    public function getImages() {
        try {
            $images = array(
                "http://image.spreadshirt." . $this->config->getDomain() . "/image-server/v1/products/" . $this->pid . "/views/1,width=300,height=300.png",
                "http://image.spreadshirt." . $this->config->getDomain() . "/image-server/v1/compositions/" . $this->pid . "/views/1,width=300,height=300.png"
            );
            foreach ($images as $key => $image) {
                $content = @file_get_contents($image);
                if ($content !== FALSE) {
                    $filename = preg_replace("/(\W+|\s+ )/", '-', $this->name) . $this->id . "-$key.png"; //subtitutes whitespaces for '-'
                    if (@file_put_contents("images/$filename", $content) !== FALSE) {
                        $this->{"img" . $key} = ($filename); //this sets the objects image field after file is written to disk
                    } else {
                        throw new Exception("Unable to write image file " . $filename . " to disk for article " . $this->id);
                    }
                } else {
                    throw new Exception("Unable to get image files from spreadshirt");
                }
            }
            return TRUE;
        } catch (Exception $exc) {
            $log = new SprdCMSLogger();
            $log->logEvent($exc);
            return FALSE;
        }
    }

}

