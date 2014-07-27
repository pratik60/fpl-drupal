<?php

/**
 * Configure the webform_to_gdocs module.
 */
function webform_to_gdocs_configure_form($form, &$form_state) {
  $form['gdocs_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Docs Username'),
    '#description' => t('The username for the Google Docs account.'),
    '#default_value' => variable_get('webform_to_gdocs_username', ''),
    '#required' => TRUE,
  );
  $form['gdocs_password'] = array(
    '#type' => 'password',
    '#title' => t('Google Docs Password'),
    '#description' => t('The password for the Google Docs accoubt. The password will not be displayed once it is saved, and will only be overwritten if you enter something here.'),
  );
  $form['zend_gdata_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Zend GData Library Location'),
    '#description' => t('The path to the Zend GData library, relative to the Drupal root (ex: <code>sites/all/libraries/zend_gdata</code>).'),
    '#default_value' => variable_get('webform_to_gdocs_zend_gdata_location', 'sites/all/libraries/zend_gdata'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Save Settings',
  );

  return $form;
}

function webform_to_gdocs_configure_form_submit(&$form, &$form_state) {
  variable_set('webform_to_gdocs_username', $form_state['values']['gdocs_username']);
  variable_set('webform_to_gdocs_zend_gdata_location', $form_state['values']['zend_gdata_location']);

  // Only update password if they've entered something.
  if (!empty($form_state['values']['gdocs_password'])) {
    variable_set('webform_to_gdocs_password', $form_state['values']['gdocs_password']);
  }

  drupal_set_message('Settings updated.');

  // Verify the Zend Gdata library can be loaded.
  _webform_to_gdocs_helper_loadZendGdata();
}
