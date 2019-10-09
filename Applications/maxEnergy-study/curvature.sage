var('k','x','y','R','E')

# k est la courbure (positive alors parabole positive, donc concave ici)
# R est la distance au contours des centres des boules d'intersection ext et int
# R+E est le rayon de ces boules, E > 0

f(x)=1/2*k*x^2
ge(x)=R-sqrt((R+E)^2-x^2)
gi(x)=-R+sqrt((R+E)^2-x^2)

#solve( ge(x)=f(x), x)
# ne marche pas

# xe est l'abscisse où le contour et la boule externe s'intersectent
De=(1-k*R)^2+k^2*(2*R*E+E^2)
xe(k,E,R)=sqrt(2)/k*sqrt(k*R-1+sqrt(De))

# xi est l'abscisse où le contour et la boule externe s'intersectent
Di=(1+k*R)^2+k^2*(2*R*E+E^2)
xi(k,E,R)=sqrt(2)/k*sqrt(-k*R-1+sqrt(Di))

Je=plot(xe(0.25,E,1), (E,0,0.1),color='red')
Ji=plot(xi(0.25,E,1), (E,0,0.1),color='blue')

# On calcule l'aire entre la courbe et les boules
Ae=integral( 2*(f(x) - ge(x)), (x,0, xe(k,E,R)) )
Ai=integral( 2*(gi(x) - f(x)), (x,0, xi(k,E,R)) )

# On calcule le défaut d'aire par rapport à pi*r^2
Ee=(pi*(R+E)^2-Ae)^2
Ei=(pi*(R+E)^2-Ai)^2

# Si on fait que Ee ou Ei il y a un terme en k, sinon terme en k^2 à l'ordre 4.
Energy(k,E,R)=taylor(Ee+Ei,(E,0),(k,0),4)

beta(E,R)=-2*sqrt(2)*pi*E^(3/2)*R^(9/2)
alpha(k,E,R)=Energy(k,E,R)-beta(E,R)*k^2


# -2*sqrt(2)*pi*E^(3/2)*R^(9/2)*k^2 + 12*pi^2*E^2*R^2 + 8*pi^2*E*R^3 + 2*pi^2*R^4
# - 611/70*sqrt(2)*pi*E^(7/2)*sqrt(R) - 188/15*sqrt(2)*pi*E^(5/2)*R^(3/2) - 16/3*sqrt(2)*pi*E^(3/2)*R^(5/2)
# + 2/45*(45*pi^2 + 112)*E^4 + 8/9*(9*pi^2 + 8)*E^3*R


# Plus k est grand plus l'énergie est faible, du coup il faut maximiser.
