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

        scope.changeMode = function() {
          Arduino.setPins({
            'pins': [{
              pin: scope.ngModel.pin,
              mode: 0 // input
            }]
          }).then(function(pins) {
            console.log('pins', pins);
          });
        };

        scope.runAction = function() {
          Arduino.setPins({
            'pins': [{
              pin: scope.ngModel.pin,
              action: 'getTemp'
            }]
          }).then(function(pin) {
            console.log('pins', pin.data);
            scope.ngModel = pin.data;
          });
        };

        scope.toggle = function() {
          if (pinState === 0) {pinState = 1;}
          else {pinState = 0;}

          var data = {
            'pins': [{
              pin: scope.ngModel.pin,
              value: pinState
            }]
          };
          Arduino.setPins(data)
          .then(function() {
            // scope.pins = pins;
            for (var i = 0; i < data.pins.length; i++) {
              var pin = data.pins[i];
              if (pin.pin === scope.ngModel.pin) {
                scope.ngModel.pin = pin.pin;
                scope.ngModel.value = pin.value;
              }
            }
          });
        };

      }
    };
  });
