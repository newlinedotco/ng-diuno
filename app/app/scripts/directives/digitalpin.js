'use strict';

angular.module('myApp')
  .directive('digitalPin', function(HOSTED_URL, Arduino) {
    return {
      templateUrl: HOSTED_URL() + '/views/digitalPin.html',
      restrict: 'A',
      scope: {
        'ngModel': '='
      },
      link: function postLink(scope) {
        var pinState = scope.value;

        scope.toggle = function() {
          if (pinState === 0) {pinState = 1;}
          else {pinState = 0;}

          console.log(scope.ngModel);
          var data = {
            'pins': [{
              pin: scope.ngModel.pin,
              value: pinState
            }]
          };
          console.log(data);
          Arduino.setPins(data)
          .then(function() {
            // scope.pins = pins;
          });
        };

      }
    };
  });
