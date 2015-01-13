<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * This class create instances of a valid store articles build from manual input of a unique spreadshirt article ID.
 *
 * This class hold an instance of a valid item build from manual input. It is not fully implemented yet and should not be used.
 *
 * @see SprdCMSItem
 * @package   SprdCMS
 * @todo This class is not implemented yet, it will be when the full integration with the spreadshirt REST API is added.
 */
class SprdCMSItemManual Extends SprdCMSItem {
//    protected $config, $xml;

    /**
     * Builds a valItem to be saved into database from a unique ID input. Queries Spreadshirt API server to retreive all of the information.
     *
     * @param string $itemId The unique ID that matches a Spreadshirt article to be added to the store.
     * @todo    Implement method and rewrite logic.
     */
    public function __construct($itemId) {
        parent::__construct();
//        $this->config = SprdCMSSiteConfig::getConfig();
//        $this->item = $item;
//        $this->shopId = $this->config->getShopId();
//        $this->shopDomain = $this->config->getDomain();
//        $this->binded = array(':id' => $this->item,
//            ':name' => $this->xml->name,
//            ':img0' => preg_replace("/(\W+|\s+ )/", '-', $this->xml->name) . "-0-" . ".png",
//            ':img1' => preg_replace("/(\W+|\s+ )/", '-', $this->xml->name) . "-1-" . ".png",
//            ':price' => $this->xml->price->vatIncluded,
//        );
    }

    /**
     * This method dynamically loads xml files for the current article.
     * The xml file is obtained by querying the REST API using the applicationg wide configuration values.
     *
     * @todo Implement the method correctly.
     */
    private function getXml() { //needs more error checking
        try {
            $this->xml = @simplexml_load_file('http://api.spreadshirt.' . $this->shopDomain . '/api/v1/shops/' . $this->shopId . '/articles/' . (string) $this->item);
        } catch (Exception $e) {
            echo ("Could not query Spreadshirt API Service<br>" . "$e->getMessage()");
        }
    }

    /**
     * Get images using the spreadshirt API
     *
     * @todo Implement the method correctly.
     */
    public function getImages() {
//        $this->img = array(
//            "http://image.spreadshirt." . $this->shopDomain . "/image-server/v1/products/" . $this->xml->product['id'] . "/views/1,width=300,height=300.png",
//            "http://image.spreadshirt." . $this->shopDomain . "/image-server/v1/compositions/" . $this->xml->product['id'] . "/views/1,width=300,height=300.png"
//        );
//        try {
//            foreach ($this->img as $key => $imgLink) {
//                $content = file_get_contents($imgLink);
//                $filename = preg_replace("/(\W+|\s+ )/", '-', $this->xml->name) . $key . ".png";
//                $savePath = "images/" . $filename;
//                file_put_contents($savePath, $content);
//            }
//        } catch (Exception $e) {
//            echo "Couldn't save images to disk" . $e->getMessage();
//        }
    }

}

