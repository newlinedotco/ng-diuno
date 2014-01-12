'use strict';

angular.module('myApp')
  .directive('mainView', function(HOSTED_URL, Ardunio) {
    return {
      templateUrl: HOSTED_URL() + '/views/home.html',
      restrict: 'A',
      link: function(scope) {
        var pinState = 0;
        scope.toggle = function() {
          if (pinState === 0) {pinState = 1;}
          else {pinState = 0;}

          Ardunio.setPins({13: pinState}).then(function(pins) {
            scope.pins = pins;
          });
        };

        Ardunio.getPins()
        .then(function(pins) {
          scope.pins = pins;
        });
      }
    };
  });
