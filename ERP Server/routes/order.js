var debug = require('debug')('erp-server:orders');
var createError = require('http-errors');
var express = require('express');
var router = express.Router();

var pendingOrders = [];
var orders = [];

var validStatus = ['pending', 'processing', 'processed', 'deferred'];
var woodTypes = ['Ash', 'Oak', 'Fichte', 'Fir', 'Larch', 'Pine', 'Spruce', 'Walnut'];

var hwm = 7;
var lwm = 2;

function generateNewOrders() {
  debug(`generating ${hwm - pendingOrders.length} new orders`);
  while (pendingOrders.length < hwm) {
    var next = {
      id: orders.length,
      wood: woodTypes[Math.floor(Math.random() * woodTypes.length)],
      quantity: Math.round(Math.random() * 10),
      dimensions: {
        height: Math.round(Math.random() * 5),
        width: Math.round(Math.random() * 240),
        length: Math.round(Math.random() * 500),
      },
      status: 'pending',
      complete: false,
    };
    pendingOrders.push(next);
    orders.push(next);
  }
}

/* GET list of pending orders */
router.get('/', function (req, res, next) {
  var status = req.query.status;

  if (status == null || status.length === 0) {
    res.send(orders);
  } else if (validStatus.reduce((all, str) => all && status.indexOf(str) >= 0, true)) {
    res.send(orders);
  } else if (status.length === 1 && status[0] === 'pending') {
    res.send(pendingOrders);
  } else {
    res.send(orders.filter((order) => status.indexOf(order.status) >= 0));
  }
});
/* GET details for one order */
router.get('/:id', function (req, res, next) {
  var id = Number(req.params.id);
  if (!Number.isInteger(id)) {
    debug(`'${id}' is not an integer`);
    next(createError(400));
    return;
  }
  if (id < 0 || id >= orders.length) {
    next(createError(404));
    return;
  }
  res.send(orders[id]);
});
/* PATCH next status in order */
router.patch('/:id', function (req, res, next) {
  var id = Number(req.params.id);
  if (!Number.isInteger(id)) {
    debug(`'${id}' is not an integer`);
    next(createError(400));
    return;
  }
  id = Number.parseInt(id);
  if (id < 0 || id >= orders.length) {
    next(createError(404));
    return;
  }

  if (req.body.status == null || validStatus.indexOf(req.body.status) < 0) {
    next(createError(405));
    return;
  }

  var order = orders[id];

  switch (req.body.status) {
    case 'processing':
      if (order.status !== 'pending') {
        next(createError(409));
        return;
      }
      break;
    case 'deferred':
    case 'processed':
      if (order.status !== 'processing') {
        next(createError(409));
        return;
      }
      break;
    default:
      next(createError(409));
      return;
  }

  debug(`changing status from '${order.status}' to '${req.body.status}' for order ${id}`);
  var pIndex = order.status === 'pending' ? pendingOrders.indexOf(order) : -1;
  if (pIndex >= 0) {
    pendingOrders.splice(pIndex, 1);
  }

  if (req.body.status === 'deferred') {
    if (order.status !== 'processing') {
      next(createError(409));
      return;
    }
    order.status = 'pending';
    pendingOrders.push(order);
  } else {
    order.status = req.body.status;
  }

  res.send(order);

  if (pendingOrders.length < lwm) {
    generateNewOrders();
  }
});

generateNewOrders();
module.exports = router;
