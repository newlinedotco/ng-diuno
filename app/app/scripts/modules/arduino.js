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
    var service = {
      setupPins: function(pins) {
        return $http({
          method: 'POST',
          url: rootUrl + '/pins/setup',
          data: pins
        });
      },
      getPins: function() {
        return $http({
          method: 'GET',
          url: rootUrl + '/pins'
        }).then(function(data) {
          return data.data;
        });
      },
      setPins: function(pins) {
        return $http({
          method: 'POST',
          url: rootUrl + '/pins/digital',
          data: pins
        });
      }
    };

    return service;
  };
});