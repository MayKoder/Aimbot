function [ ] = Aimbot_GUI ( )
%Aimbot_GUI Aimbot, Graphical User Interface
%   Automatic aim bot: find shooting conditions to hit a target
%   2D particle physics integrator based on 2n order Verlet scheme
%
%David de la Torre
%December 2019

%% Configuration parameters
% These are the scenario configuration variables; modify as desired.

% Simulation options
opts.m = 1.0; % Particle mass [kg]
opts.S = 0.1; % Particle front surface [m^2]
opts.Cd = 1.0; % Particle drag coefficient [-]
opts.bc = 0.95; % Particle bounce coefficient / restoration factor [0-1]
opts.rho = 1.225; % World atmosphere density [kg/m3]
opts.g0 = [0.0,-9.81]; % World gravity acceleration (x,y) [m/s2]
opts.wind = [10,0]; % World wind velocity vector (vx,vy) [m/s]
opts.ww = 20; % World scenario width [m]
opts.wh = 10; % World scenario height [m]
opts.fps = 30; % Game frames per second [fps]
opts.tf = 5; % Game final simulation time [seconds]

% Target options
% The target is assumed to be static
% The target hitbox is assumed spherical for simplicity
opts.target.pos = [15.0,5.0]; % Target position (x,y) [m]
opts.target.hbx = 0.5; % Target hitbox (spherical) radius [m]

% Shooter position
% The shooter is assumed to be static
x = [1.0,1.0]; % Target position (x,y) [m]

% Monte-Carlo options
n = 100; % Number of Monte-Carlo samples [#]
vns = [0,50]; % Control values range: velocity (min,max) [m/s]
ths = [-180,180]; % Control values range: angle (min,max) [deg]

% Matlab figure size
figW = 900; % Figure width [px]
figH = 350; % Figure height [px]

%% Build Matlab GUI figure
% The figure will have 1x8 partitions:
%   Partitions 1:2 (left) will allocate the configuration sliders
%   Partitions 3:8 (right) will allocate the main simulation plot

% Create new figure
fh = figure();

% Resize figure to user-configured dimensions
% The subroutine GetFigCC returns the position (x0,y0,w,h) of the figure
% centered on the computer's screen
fh.Position = GetFigCC ( figW, figH );

% Create scenario subplot
ah = subplot(1,8,3:8,'Nextplot','add'); % Main simulation plot
grid on; box on; axis equal; % Plot options
title('AIMBOT | Test sandbox'); % Title
xlabel('Position X [m]'); % Label of x-axis
ylabel('Position Y [m]'); % Label of y-axis
xlim([0,opts.ww]); % Limits of x-axis: size of scenario
ylim([0,opts.wh]); % Limits of y-axis: size of scenario

% Slider parameters
sliW0 = 20; sliH0 = 30; % Initial reference width/height [px]
sliLabW = 50; % Slider Label width [px]
SliW = 100; % Slider width [px]

% Sliders Label
z = sliH0;
uicontrol('style','text','units','pixels',...
    'position',[sliW0+sliLabW+10,figH-z-20,SliW,20],...
    'String','CONTROLS','HorizontalAlignment','center'); % Title label
z = z + 50;
uicontrol('style','text','units','pixels',...
    'position',[sliW0,figH-z,sliLabW,20],...
    'String','Velocity','HorizontalAlignment','right'); % Velocity label
z = z + 50;
uicontrol('style','text','units','pixels',...
    'position',[sliW0,figH-z,sliLabW,20],...
    'String','Angle','HorizontalAlignment','right'); % Angle label
z = z + 50;
uicontrol('style','text','units','pixels',...
    'position',[sliW0,figH-z,sliLabW,20],...
    'String','Wind','HorizontalAlignment','right'); % Wind label

% Sliders
z = sliH0;
z = z + 50;
slider_vn = uicontrol('style','slider','units','pixels',...
    'position',[sliW0+sliLabW+10,figH-z,SliW,20],...
    'Min',0,'Max',50,'Value',10,'SliderStep',[1/50,5/50],...
    'callback',@sliderCallback); % Velocity slider
z = z + 50;
slider_th = uicontrol('style','slider','units','pixels',...
    'position',[sliW0+sliLabW+10,figH-z,SliW,20],...
    'Min',-180,'Max',180,'Value',45,'SliderStep',[5/360,45/360],...
    'callback',@sliderCallback); % Angle slider
z = z + 50;
slider_vw = uicontrol('style','slider','units','pixels',...
    'position',[sliW0+sliLabW+10,figH-z,SliW,20],...
    'Min',-10,'Max',10,'Value',5,'SliderStep',[1/20,5/20],...
    'callback',@sliderCallback); % Wind slider
z = z + 50;
uicontrol('style','pushbutton','units','pixels',...
    'position',[sliW0+sliLabW+10,figH-z,SliW,20],...
    'String','Monte-Carlo',...
    'callback',@monteCarloCallback); % Monte-Carlo button
    
% Sliders Text
z = sliH0;
z = z + 50;
textval_vn = uicontrol('style','text','units','pixels',...
    'position',[sliW0+sliLabW+10+SliW+10,figH-z,50,20],...
    'String','0 m/s','HorizontalAlignment','left'); % Velocity value
z = z + 50;
textval_th = uicontrol('style','text','units','pixels',...
    'position',[sliW0+sliLabW+10+SliW+10,figH-z,50,20],...
    'String','0 deg','HorizontalAlignment','left'); % Angle value
z = z + 50;
textval_vw = uicontrol('style','text','units','pixels',...
    'position',[sliW0+sliLabW+10+SliW+10,figH-z,50,20],...
    'String','0 m/s','HorizontalAlignment','left'); % Wind value

% Plot shooter
cfg_shoo = {'k^','MarkerSize',8,'LineWidth',2}; % Style: black triangle
plot(ah,x(1),x(2),cfg_shoo{:}); % Plot shooter marker

% Plot ball
cfg_ball = {'bo'}; % Style: blue circles
ph_ball = plot(ah,0,0,cfg_ball{:}); % Placeholder for ball data

% Plot bounces
cfg_boun = {'mx'}; % Style: magenta cross
ph_boun = plot(ah,0,0,cfg_boun{:}); % Placeholder for bounces data

% Plot target hit
cfg_hit = {'rx','MarkerSize',20,'LineWidth',2}; % Style: red cross
ph_hit = plot(ah,0,0,cfg_hit{:}); % Placeholder for hit marker data

% Plot target
cfg_targ = {'k.','MarkerSize',1,'LineWidth',1}; % Style: black dots
tx = opts.target.pos(1) + opts.target.hbx * cos(linspace(0,2*pi,1E2));
ty = opts.target.pos(2) + opts.target.hbx * sin(linspace(0,2*pi,1E2));
plot(ah,tx,ty,cfg_targ{:}); % Plot target hitbox (circle) at scale

% Ball historical trajectories (misses)
cfg_hmiss = {'-','Color',[0.9,0.9,0.9]}; % Style: light grey line
ph_hmiss = gobjects(n,1); % Preallocate array of graphic objects
ph_hmiss(1) = plot(ah,0,0,cfg_hmiss{:}); % Placeholder for misses data

% Ball historical trajectories (hits)
cfg_hhit = {'g-','LineWidth',2}; % Style: green thick line
ph_hhit = gobjects(n,1); % Preallocate array of graphic objects
ph_hhit(1) = plot(ah,0,0,cfg_hhit{:}); % Placeholder for hits data

% Plot data
% Manually-calling the slider handle here allows plotting everything the
% first time this program is called. Kind of "initializing" the scenario.
sliderCallback([],[],[]);

%% Internal subroutines of the GUI

    % Simulate the current scenario
    function simulateScenario ( mc )
    %simulateScenario
    %   - Gets all the current values from the sliders
    %   - Computes all the required input magnitudes (velocity,etc.)
    %   - Simulates the scenario (integrates phys for all frames)
    %   - Plots the results in the GUI
    %
    % Inputs:
    %   mc: if >0, we are doing a Monte-Carlo analysis. The value of mc
    %       corresponds to the current index on the Monte-Carlo for-loop
    %
    % Outputs:
    %   n/a
    %
    %David de la Torre
    %December 2019
    
        % Get slider values
        vn = slider_vn.Value; % Velocity magnitude
        th = slider_th.Value; % Velocity angle
        vw = slider_vw.Value; % Wind magnitude

        % Update slider text
        set(textval_vn,'String',sprintf('%.0f m/s',vn)); % Velocity
        set(textval_th,'String',sprintf('%.0f deg',th)); % Angle
        set(textval_vw,'String',sprintf('%.0f m/s',vw)); % Wind
        
        % Build particle velocity vector
        v = vn * [cosd(th),sind(th)];
        
        % Update world wind vector
        % Assumed to be fixed in x-axis
        % The GUI can be upgraded in the future to accommodate 2D wind
        opts.wind = [vw,0];
        
        % Integrate trajectory
        % Use 2D Verlet (2nd order) integrator
        [ flag, xh, ch ] = Aimbot_Verlet ( x, v, opts );
        
        % Delete old plot handles
        delete(ph_ball); % Trajectory
        delete(ph_boun); % Bounces
        delete(ph_hit); % Target hit
        
        % Plot new ball trajectory (position at each frame)
        ph_ball = plot(xh(:,1),xh(:,2),cfg_ball{:}); % Ball
        ph_boun = plot(xh(ch>0,1),xh(ch>0,2),cfg_boun{:}); % Bounces
        
        % Plot cross marker at the hit point with target, if required
        if flag > 0 % Hit!
            ph_hit = plot(xh(flag,1),xh(flag,2),cfg_hit{:}); % Plot marker
        else % Not hit
            ph_hit = plot(-10,-10,'k.'); % Plot marker outside scenario
        end
        
        % Plot historical of ball trajectories, if Monte-Carlo
        if mc > 0 % We are doing a Monte-Carlo analysis
            if flag > 0 % Hit!
                ph_hhit(mc) = plot(ah,xh(:,1),xh(:,2),cfg_hhit{:});
            else % Not hit
                ph_hmiss(mc) = plot(ah,xh(:,1),xh(:,2),cfg_hmiss{:});
            end
        end

        % Flush graphics
        % This ensures all pending graphic operations are plotted now
        drawnow;

    end

    % Monte-Carlo analysis
    function monteCarloAnalysis ( )
    %monteCarloAnalysis
    %   - Obtains n random samples of the velocity (magnitude and angle)
    %   - For each sample, simulate the scenario (integrate and plot)
    %   - Post-process?
    %
    % Inputs:
    %   n/a
    %
    % Outputs:
    %   n/a
    %
    %David de la Torre
    %December 2019
        
        % Get n random samples and simulate each one
        for k=1:n
            
            % Select random values of the control parameters
            vn = vns(1) + diff(vns) * rand(1); % Velocity magnitude
            th = ths(1) + diff(ths) * rand(1); % Velocity angle
            
            % Set slider values
            % We'll set the values now, then they will be processed within
            % simulateScenario subroutine automatically.
            slider_vn.Value = vn; % Velocity magnitude
            slider_th.Value = th; % Velocity angle

            % Update scenario
            % This function will process the current values of vn and th
            % The value of k will be used to:
            %   - Tell simulateScenario that we are doing a MC analysis
            %   - Plot the trajectory of the ball in the k-position of the
            %     array of graphic objects where we are saving all the
            %     "historical trajectories".
            simulateScenario ( k );

            % Post-process
            % How good are the solutions?
            % Can we select one of them?
            
        end
        
    end

    % Slider callback: Velocity, Angle
    function sliderCallback(~, ~, ~)
    %sliderCallback
    %   - The values of the sliders have changed: process new values
    %
    % Inputs:
    %   n/a (not required, irrelevant in this function)
    %
    % Outputs:
    %   n/a
    %
    %David de la Torre
    %December 2019
        
        % Delete leftover plot data from Monte-Carlo analysis
        % Quite dirty; the placement of this code could be improved
        for i=1:length(ph_hmiss), delete(ph_hmiss(i)); end
        for i=1:length(ph_hhit), delete(ph_hhit(i)); end
        
        % Update frame
        % The new values of the sliders will be processed within the
        % simulateScenario subroutine
        % The value of 0 indicates we are NOT doing a MC analysis
        simulateScenario ( 0 );
        
    end

    % Button callback: Monte-Carlo
    function monteCarloCallback(~, ~, ~)
    %monteCarloCallback
    %   - The Monte-Carlo button has been pressed: execute MC
    %
    % Inputs:
    %   n/a (not required, irrelevant in this function)
    %
    % Outputs:
    %   n/a
    %
    %David de la Torre
    %December 2019
        
        % Delete leftover plot data from Monte-Carlo analysis
        % Quite dirty; the placement of this code could be improved
        for i=1:length(ph_hmiss), delete(ph_hmiss(i)); end
        for i=1:length(ph_hhit), delete(ph_hhit(i)); end
        
        % Launch Monte-Carlo analysis
        monteCarloAnalysis();
        
    end

end


%% Aimbot Verlet
function [ flag, xh, ch ] = Aimbot_Verlet ( x, v, opts )
%Aimbot_Verlet
%   2D particle physics integrator based on 2n order Verlet scheme
%   Subroutine tuned for usage with the Aimbot GUI script; here only the
%   trajectory is computed and no graphics are produced.
%
% Inputs:
%   x: initial position vector (x,y)
%   v: initial velocity vector (vx,vy)
%
% Outputs:
%   flag: 0 if no hit, >0 if hit (actual value is the #frame at the hit)
%   xh: historical of position of the ball throughout the simulation
%   ch: historical of bounces of the ball throughout the simulation
%
%David de la Torre
%December 2019

% Configuration parameters
m = opts.m; % Particle mass [kg]
S = opts.S; % Particle front surface [m^2]
Cd = opts.Cd; % Particle drag coefficient [-]
bc = opts.bc; % Particle bounce coefficient / restoration factor [0-1]
rho = opts.rho; % World air density [kg/m3]
g0 = opts.g0; % World gravity acceleration [m/s2]
wind = opts.wind; % World wind velocity vector [m/s]
ww = opts.ww; % World width [m]
wh = opts.wh; % World height [m]
fps = opts.fps; % Game frames per second [fps]
tf = opts.tf; % Game final simulation time [seconds]
ff = tf * fps; % Game total simulation frames (assume constant fps) [#]

% Preallocate arrays
xh = zeros(ff,2); % Particle position history (x,y) [m]
ch = zeros(ff,1); % Bounce points history [0/1]

% Initial flag value: 0 (no collision with target)
flag = 0;

% Frame loop
for i=1:ff % Advance frames
    
    % Sync game-global fps -> compute frame time step
    dt = 1.0 / fps;
    
    % Compute gravity force on particle
    Fg = m * g0;
    
    % Compute aerodynamic drag force on particle
    vw = v - wind; % Particle velocity (wind reference frame)
    vu = vw / norm(vw); % Unitary particle-wind velocity vector
    Fd = 0.5 * rho * vw .* vw * S * Cd .* -vu; % Drag force
    
    % Compute total force (d'Alembert principle)
    F = Fg + Fd;
    
    % Compute acceleration of particle (2nd Newton's law)
    a = F / m;
    
    % Integrate position and velocity (2nd order Verlet)
    x = x + v * dt + 0.5 * a * dt * dt;
    v = v + a * dt;
    
    % Resolve collisions with walls
    % If the particle gets out of world boundaries...
    %   - Return the particle to the position of the nearest wall
    %   - Flip the velocity component perpendicular to the wall
    %   - Apply bounce coefficient to attenuate the rebounded velocity
    %   - Set "bounce point" flag
    if x(1) < 00, x(1) = 00; v(1) = - bc * v(1); ch(i) = 1; end % Left
    if x(1) > ww, x(1) = ww; v(1) = - bc * v(1); ch(i) = 2; end % Right
    if x(2) < 00, x(2) = 00; v(2) = - bc * v(2); ch(i) = 3; end % Bottom
    if x(2) > wh, x(2) = wh; v(2) = - bc * v(2); ch(i) = 4; end % Top
    
    % Save particle history
    xh(i,:) = x(:)'; % Save particle position (x,y) [m]
    
    % Return on first bounce
    %if ch(i) > 0, break; end
    
    % Detect collision with target
    dist = norm(x(:)' - opts.target.pos(:)'); % Distance ball-target
    if dist < opts.target.hbx % Collision!
        flag = i; % Set collision flag: current frame
        break; % Stop the for loop
    end
    
end

% Reduce and clean up arrays (remove the last empty elements, if any)
xh = xh(1:i,:);
ch = ch(1:i,:);

end


%% GetFigCC
function [ pos ] = GetFigCC ( w, h )
%GETFIGCC Get Figure Centered Coordinates
%   Returns vector with coordinates [W0,H0,W,H] of a centered figure.
%
% Inputs:
%   w: figure width [px]
%   h: figure height [px]
%
% Outputs:
%   pos: [w0, h0, w, h]
%
% Example:
%   [ pos ] = GetFigCC ( 800, 600 );
%
% References:
% n/a
%
%David de la Torre Sangra
%August 2014

% Detect inputs
if nargin==1 && length(w)==2 % Single input with both magnitudes
    wh = w; % Save input array
    clear w; % Clear variable
    w = wh(1); % Get width
    h = wh(2); % Get height
elseif nargin < 2 % Too few inputs
    w = 600; % Set default width
    h = 400; % Set default height
end

% Get screen dimensions [pixels]
ss = get(0,'ScreenSize');

% Initial coordinates, centered on the screen
w0 = floor(ss(3)/2 - w/2); % Width-magnitude stating point
h0 = floor(ss(4)/2 - h/2); % Height-magnitude starting point
if h0 < 10, h0 = 10; end % Avoid figures out of screen
if w0 < 10, w0 = 10; end % Avoid figures out of screen

% Construct vector to be used in 'Position' property of figures
pos = [w0, h0, w, h];

end

