"""
Shallow Water
-------------

So what does this code example illustrate?

Adapted from: http://people.sc.fsu.edu/~jburkardt/m_src/shallow_water_2d/
"""
import bohrium as np

g = 9.80665 # gravitational acceleration

def model(height, width, dtype=np.float32, bohrium=True):
    m = np.ones((height, width),dtype=dtype,bohrium=bohrium)   
    m[height/4,width/4] = 6.0
    return m

def step(H, U, V, dt=0.02, dx=1.0, dy=1.0):
    # Reflecting boundary conditions
    H[:,0] = H[:,1]   ; U[:,0] = U[:,1]     ; V[:,0] = -V[:,1]
    H[:,-1] = H[:,-2] ; U[:,-1] = U[:,-2]   ; V[:,-1] = -V[:,-2]
    H[0,:] = H[1,:]   ; U[0,:] = -U[1,:]    ; V[0,:] = V[1,:]
    H[-1,:] = H[-2,:] ; U[-1,:] = -U[-2,:]  ; V[-1,:] = V[-2,:]

    #First half step

    # height
    Hx = (H[1:,1:-1]+H[:-1,1:-1])/2 - dt/(2*dx)*(U[1:,1:-1]-U[:-1,1:-1])
    
    # x momentum
    Ux = (U[1:,1:-1]+U[:-1,1:-1])/2 - \
         dt/(2*dx) * ((U[1:,1:-1]**2/H[1:,1:-1] + g/2*H[1:,1:-1]**2) - 
                      (U[:-1,1:-1]**2/H[:-1,1:-1] + g/2*H[:-1,1:-1]**2))

    # y momentum
    Vx = (V[1:,1:-1]+V[:-1,1:-1])/2 - \
         dt/(2*dx) * ((U[1:,1:-1]*V[1:,1:-1]/H[1:,1:-1]) - 
                      (U[:-1,1:-1]*V[:-1,1:-1]/H[:-1,1:-1]))

    # height
    Hy = (H[1:-1,1:]+H[1:-1,:-1])/2 - dt/(2*dy)*(V[1:-1,1:]-V[1:-1,:-1])

    #x momentum
    Uy = (U[1:-1,1:]+U[1:-1,:-1])/2 - \
         dt/(2*dy)*((V[1:-1,1:]*U[1:-1,1:]/H[1:-1,1:]) - 
                    (V[1:-1,:-1]*U[1:-1,:-1]/H[1:-1,:-1]))
    #y momentum
    Vy = (V[1:-1,1:]+V[1:-1,:-1])/2 - \
         dt/(2*dy)*((V[1:-1,1:]**2/H[1:-1,1:] + g/2*H[1:-1,1:]**2) - 
                    (V[1:-1,:-1]**2/H[1:-1,:-1] + g/2*H[1:-1,:-1]**2))

    #Second half step

    # height
    H[1:-1,1:-1] -= (dt/dx)*(Ux[1:,:]-Ux[:-1,:]) + (dt/dy)*(Vy[:,1:]-Vy[:,:-1])

    # x momentum
    U[1:-1,1:-1] -= (dt/dx)*((Ux[1:,:]**2/Hx[1:,:] + g/2*Hx[1:,:]**2) - 
                             (Ux[:-1,:]**2/Hx[:-1,:] + g/2*Hx[:-1,:]**2)) + \
                    (dt/dy)*((Vy[:,1:] * Uy[:,1:]/Hy[:,1:]) - 
                             (Vy[:,:-1] * Uy[:,:-1]/Hy[:,:-1]))
    # y momentum
    V[1:-1,1:-1] -= (dt/dx)*((Ux[1:,:] * Vx[1:,:]/Hx[1:,:]) -
                             (Ux[:-1,:]*Vx[:-1,:]/Hx[:-1,:])) + \
                    (dt/dy)*((Vy[:,1:]**2/Hy[:,1:] + g/2*Hy[:,1:]**2) - 
                             (Vy[:,:-1]**2/Hy[:,:-1] + g/2*Hy[:,:-1]**2))
                    
    return (H, U, V)

def simulate(H, timesteps):
    U = np.zeros_like(H)
    V = np.zeros_like(H)
    for i in xrange(timesteps):
        (H, U, V) = step(H, U, V)
    return H