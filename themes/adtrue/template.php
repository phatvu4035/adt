<?php
/**
 * @file
 * The primary PHP file for this theme.
 */
function adtrue_theme($existing, $type, $theme, $path) {
    return array(
        'slides_node_form' =>
            array(
                'arguments' => array('form' => NULL),
                'template' => 'templates/slides_add_form',
                'render element' => 'form',
            ),
  		);
}