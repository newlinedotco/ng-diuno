'use strict';

angular.module('fsArduino', [])
.config(function($httpProvider) {
  var interceptor = function() {
    return {
      'request': function(req) {
        // req.data = JSON.stringify(req.data)+'\n';
        return req;
      }
    };
  };
  $httpProvider.interceptors.push(interceptor);
})
.provider('Arduino', function() {

  var rootUrl = 'http://10.0.1.32';

  this.setRootUrl = function(u) {
    rootUrl = u || rootUrl;
  };

  this.$get = function($q, $http) {
    var actions = {
      'getTemp': 0
    };

    var actionifyPins = function(pins) {
      var str = '';
      for (var i = 0; i < pins.length; i++) {
        var p = pins[i];
        str += 'p' + p.pin;
        if (typeof(p.mode) !== 'undefined') {str += 'm' + p.mode;}
        if (typeof(p.value) !== 'undefined') {str += 'v' + p.value;}
        if (typeof(p.action) !== 'undefined') {str += 'a' + actions[p.action];}
      }
      return str;
    };
    var service = {
      getPins: function() {
        return $http({
          method: 'GET',
          url: rootUrl + '/pins'
        }).then(function(data) {
          return data.data;
        });
      },
      setPins: function(pins) {
        var strAction = actionifyPins(pins);
        return $http({
          method: 'POST',
          url: rootUrl + '/pins/digital',
          data: strAction,
          headers: {'X-Action-Len': strAction.length}
        }).then(function(data) {
          return data.data;
        });
      },
      subscribeToPin: function(pin) {
        return $http({
          method: 'GET',
          url: rootUrl + '/pins/subscribe/' + pin
        }).then(function(data) {
          console.log('DATA FROM SUBSCRIBE',data);
        });
      }
    };

    return service;
  };
});