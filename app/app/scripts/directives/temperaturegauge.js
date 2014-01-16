'use strict';

angular.module('myApp')
  .directive('temperatureGauge', function(D3) {
    return {
      template: '<div class="thermometer"><svg></svg></div>',
      replace: true,
      scope: {
        'ngModel': '='
      },
      restrict: 'EA',
      link: function (scope, element, attrs) {
        console.log(D3);
        var el = D3.select('.thermometer'),
            w = attrs.width || el.node().clientWidth,
            h = attrs.height || el.node().clientHeight,
            r = Math.min(w, h) / 2,
            pi = Math.PI;

        var svg = el.select('svg')
              .attr('width', w)
              .attr('height', h)
              .append('g')
              .attr('transform', 'translate(' + w/2 + ',' + h/2 + ')');

        var pie = D3.layout.pie()
          .startAngle(-pi/1.2)
          .endAngle(pi/1.2);

        var arc = D3.svg.arc()
          .innerRadius(r * 0.7)
          .outerRadius(r * 0.9);

        svg.selectAll('path.degree')
          .data(pie(D3.range(90)
          .map(function() { return 1; })))
          .enter()
            .append('path')
            .attr('class', 'degree')
            .attr('d', arc);

        var handle = svg.append('g');
        handle.append('rect')
          .attr('class', 'handle')
          .attr('width', 50)
          .attr('height', 5)
          .attr('y', -2.5)
          .attr('x', r - 65);

        var scale = D3.scale.linear()
          .domain([60,90])
          .range([-180, 0]);

        var display = svg.append('text')
            .text('72')
            .attr('y', 20);

        scope.$watch('ngModel', function(n) {
          if (!n) { return; }

          handle.transition().duration(1000)
            .attr('transform', 'rotate('+scale(n)+')');

          display.datum(n)
            .transition()
            .duration(1000)
            .tween('text', function(d) {
              var i = D3.interpolate(this.textContent, d);
              return function(t) {
                this.textContent = Math.round(i(t));
              };
            });
        });
      }
    };
  });
