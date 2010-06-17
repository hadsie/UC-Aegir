<?php

/**
 * Implementation of hook_ca_action().
 */
function uc_aegir_ca_action() {
  $actions['uc_aegir_run_task'] = array(
    '#title' => t('Run a hosting task on sites in an order'),
    '#category' => t('Order'),
    '#callback' => 'uc_aegir_action_run_task',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ), 
    ),
  );

  return $actions;
}

function uc_aegir_action_run_task(&$order, $settings) {
  foreach ($order->products as $product) {
    if (_uc_aegir_product_has_feature($product)) {
      $site = node_load($product->data['aegir_site_nid']);
      $tasklist = hosting_task_fetch_tasks($site->nid);
      if ($tasklist[$settings['task']]['task_permitted']) {
        hosting_add_task($site->nid, $settings['task']);
      }
    }
  }
}

function uc_aegir_action_run_task_form($form_state, $settings = array()) {
  $form = array();

  $tasks = array('' => t('Select'));
  foreach (hosting_available_tasks('site') as $task => $desc) {
    $tasks[$task] = $desc['title'];
  }

  $form['task'] = array(
    '#type' => 'select',
    '#title' => t('Select task to run'),
    '#default_value' => $settings['task'],
    '#options' => $tasks,
    '#required' => TRUE,
  );

  return $form;
}
