'use strict';

angular.module('myApp')
  .directive('mainView', function(HOSTED_URL, Arduino) {
    return {
      templateUrl: HOSTED_URL() + '/views/home.html',
      restrict: 'A',
      link: function(scope) {
        Arduino.getPins()
        .then(function(pins) {
          scope.pins = pins;
        });
      }
    };
  });
