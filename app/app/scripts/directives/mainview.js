'use strict';

angular.module('myApp')
  .directive('mainView', function(HOSTED_URL, Arduino) {
    return {
      templateUrl: HOSTED_URL() + '/views/home.html',
      restrict: 'A',
      link: function(scope) {
        

        scope.refresh = function() {
          Arduino.getPins()
          .then(function(pins) {
            console.log('PINS', pins);
            scope.pins = pins;
          });
        };

        scope.refresh(); // Kick it off
      }
    };
  });
