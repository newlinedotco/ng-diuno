'use strict';

angular.module('myApp')
  .directive('mainView', function(HOSTED_URL, Ardunio) {
    return {
      templateUrl: HOSTED_URL() + '/views/home.html',
      restrict: 'A',
      link: function(scope) {
        scope.toggle = function() {
          Ardunio.setPins({13: 1}).then(function(pins) {
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
