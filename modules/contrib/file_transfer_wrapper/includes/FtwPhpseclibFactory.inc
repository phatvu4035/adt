<?php

/*
 * @file
 * This file contails class definition of FtwPhpseclibFactory.
 */

namespace ftw;

use \Exception as Exception;

/**
 * Description of ftwPhpSecLibFactory
 *
 * @author SoumyaDas
 */
class FtwPhpseclibFactory {

  public static function build($connSettings) {
    try {
      // Check for phpseclib library.
      if (!class_exists('phpseclib\Crypt\RSA')) {
        throw new Exception('Phpseclib library not loaded!');
      }

      // Check for connection type variable.
      if ($connSettings['connection_type'] === '') {
        throw new Exception('Invalid Connection Type.');
      }
      else {
        // Construct appropriate class name. 
        $className = 'Ftw' . ucfirst($connSettings["connection_library"]) . ucfirst($connSettings['connection_type']);
        module_load_include('inc', 'file_transfer_wrapper', 'includes/' . $className);
        $className = 'ftw\\' . $className;

        // Assuming Class files are already loaded using autoload concept.
        if (class_exists($className)) {
          return new $className($connSettings);
        }
        else {
          throw new Exception('Connection type class not found.');
        }
      }
    }
    catch (Exception $e) {
      throw $e;
    }
  }

}
