'use strict';

angular.module('myApp')
  .directive('mainView', function($timeout, HOSTED_URL, Arduino) {
    return {
      templateUrl: HOSTED_URL() + '/views/home.html',
      restrict: 'A',
      link: function(scope) {
        var redPin = 9,
            greenPin = 8,
            temp = 7;

        var selectPin = function(n, pins) {
          for (var i = pins.length - 1; i >= 0; i--) {
            if (pins[i].pin === n) {
              return pins[i];
            }
          }
        };

        scope.refresh = function() {
          // Arduino.getPins()
          // .then(function(pins) {
          //   scope.pins = pins.pins;
          //   scope.temperature = selectPin(temp).value;
          // });
          (function poll() {
            Arduino.setPins([
              { pin: temp, action: 'getTemp' }
            ])
            .then(function(data) {
              var pins = data.pins;
              var pin = selectPin(temp, pins);
              scope.temperature = Math.round(pin.value * 100) / 100;
              // scope.temperature = Math.round(Math.random() * 100);
              $timeout(poll, 1000);
            });
          })();
        };

        scope.lowerTemp = function() {
          Arduino.setPins([
            { pin: greenPin, value: 1 }
          ]).then(function() {
            $timeout(function() {
              Arduino.setPins([
                {pin: greenPin, value: 0}
              ]);
            }, 1000);
          });
        };

        scope.raiseTemp = function() {
          Arduino.setPins([
            { pin: redPin, value: 1 }
          ]).then(function() {
            $timeout(function() {
              Arduino.setPins([
                {pin: redPin, value: 0}
              ]);
            }, 1000);
          });
        };

        scope.refresh(); // Kick it off
      }
    };
  });
