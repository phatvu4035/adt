<?php

/*
 * @file
 * This file contails class definition of FtwConnectionFactory.
 */

namespace ftw;

use \Exception as Exception;

/**
 * This will create a connection to the specified remote file server. 
 * 'qa-portal' is the custom connection name that you need to add to the above 
 * configuration page to identify a particular remote file server. Also it will 
 * switch to the remote directory that is specified in the above configuration.
 *
 * @author SoumyaDas
 */
class FtwConnectionFactory {

  public static function build($connection_name = '') {

    try {
      if ($connection_name == '') {
        throw new Exception('Invalid Connection Name.');
      }
      else {
        $connSettingsList = variable_get('file_transfer_wrapper_connections', array());

        foreach ($connSettingsList as $row) {
          if ($row["connection_name"] === $connection_name) {
            $connSettings = $row;
          }
        }

        if (!isset($connSettings)) {
          throw new Exception('Connection settings not found.');
        }

        // e.g. class FtwPhpseclibFactory.
        $libraryClassName = 'Ftw' . ucfirst($connSettings["connection_library"]) . 'Factory';
        module_load_include('inc', 'file_transfer_wrapper', 'includes/' . $libraryClassName);
        $libraryClassName = 'ftw\\' . $libraryClassName;
        // e.g. FtwPhpseclibFactory class will return FtwPhpseclibSftp class.
        $classObject = $libraryClassName::build($connSettings);

        return $classObject;
      }
    }
    catch (Exception $e) {
      watchdog_exception("FTW Error", $e, "File Transfer Wrapper", array(), WATCHDOG_ERROR);
      throw $e;
    }
  }
}
