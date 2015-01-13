<?php

/**
 * @license http://www.gnu.org/licenses/gpl-3.0.html
 * @copyright (c) 2013, J-F B
 * @author J-F B <ever.livin.never.fearful@gmail.com>
 */

/**
 * All request to the application must go through the front controller.
 *
 * All request to the application must go through the front controller.
 * Branching of logic is resolved through delegation of the SprdCMSProcessor interface passing the value of the SprdCMSRequest object.
 * @see SprdCMSProcessor
 * The class constructor needs two parameters to handle every request correctly.
 *
 * @package   SprdCMS
 */
class SprdCMSFrontController implements SprdCMSProcessor {

    /**
     * Holds a reference to an instance of SprdCMSRequest.
     * @var SprdCMSRequest
     */
    protected $request;

    /**
     * Holds a reference to an instance of SprdCMSCommandMapper.
     * @var SprdCMSCommandMapper
     */
    protected $mapper;

    /**
     * The class constructor needs two type of objects passed as parameters.
     *
     * @param SprdCMSRequest $request This is needed to decode the nature of the request, what command to issue along with any parameters to commands.
     * @see SprdCMSRequest
     * @param SprdCMSCommandMapper $mapper A transaction script that resolves request to commands or also delegates to another SprdCMSProcessor interface object.
     * @see SprdCMSCommandMapper
     * @return void
     */
    public function __construct(SprdCMSRequest $request, SprdCMSCommandMapper $mapper) {
        $this->request = $request;
        $this->mapper = $mapper;
        ;
    }

    /**
     * The value of the request object is verified.
     * In case there is no request, a default command resolving to the site's Home category is passed to the command mapper.
     *
     * @param mixed $param = NULL by default not to upset the SprdCMSProcessor interface.
     * @return void
     */
    public function process($param = NULL) {
        $checkGet = $this->request->getParameters();
        if (!empty($checkGet[0])) {
            $this->mapper->process($this->request->getParameters());
        } else {
            $this->mapper->process(array("category", "Home"));
        }
    }

}
