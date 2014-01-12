'use strict';

angular.module('fsArdunio', [])
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
.service('Ardunio', function($q, $http) {
  this.pins = {
    digital: {},
    analog: {}
  };

  this.getPins = function() {
    var d = $q.defer();
    $http({
      method: 'GET',
      url: 'http://10.0.1.32' + '/pins'
    }).success(function(data) {
      d.resolve(data);
    }, function(err) {
      console.error('error: ', err);
      d.reject(err);
    });
    return d.promise;
  };

  this.setPins = function(pins) {
    var d = $q.defer();
    $http({
      method: 'POST',
      url: 'http://10.0.1.32' + '/pins/digital',
      data: pins
    }).success(function(data) {
      console.log(data.pins.digital);
      d.resolve(data);
    }, function(err) {
      console.error('ERR', err);
      d.reject(err);
    });
    return d.promise;
  };

});
