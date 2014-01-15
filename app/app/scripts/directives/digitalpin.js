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
        var pinState = scope.ngModel.value;

        var findByPinNumber = function(pins) {
          for (var i = 0; i < pins.length; i++) {
            if (pins[i].pin === scope.ngModel.pin) { return pins[i]; }
          }
        };

        scope.changeMode = function() {
          Arduino.setPins([{
              pin: scope.ngModel.pin,
              mode: 0 // input
            }]).then(function(pins) {
              console.log('pins', pins);
            });
        };

        scope.runAction = function() {
          Arduino.setPins([{
              pin: scope.ngModel.pin,
              action: 'getTemp'
            }]).then(function(data) {
              console.log('pins', findByPinNumber(data.pins));
              scope.ngModel = findByPinNumber(data.pins);
            });
        };

        scope.toggle = function() {
          if (pinState === 0) {pinState = 1;}
          else {pinState = 0;}

          var data = [{
            pin: scope.ngModel.pin,
            value: pinState
          }];
          Arduino.setPins(data)
          .then(function(data) {
            var pin = findByPinNumber(data.pins);
            scope.ngModel = pin;
          });
        };

      }
    };
  });
