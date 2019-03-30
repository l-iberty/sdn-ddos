# --custom=mytopo.py --topo=mytopo

from mininet.topo import Topo
class MyTopo( Topo ):
	"Simple topology example."
	
	def __init__( self ):
		"Create custom topo."
		"""
             [h2]          [h5]          [h8]
         [h1] |  [h3]  [h4] |  [h6]  [h7] |  [h9]
           \  |  /       \  |  /       \  |  /
            \ | /         \ | /         \ | /
             [s1]----------[s2]----------[s3]
                            |
                            |
                           [s4]
                         /      \
                        /        \
                       /          \
                    [s5]          [s6]
                   / | \          / | \
                  /  |  \        /  |  \
                 /   |   \   [h13]  |   [h15]
             [h10]   |   [h12]    [h14]
                   [h11]
		"""

		Topo.__init__(self)
		# add hosts
		h1 = self.addHost( 'h1' )	
		h2 = self.addHost( 'h2' )	
		h3 = self.addHost( 'h3' )	
		h4 = self.addHost( 'h4' )	
		h5 = self.addHost( 'h5' )	
		h6 = self.addHost( 'h6' )	
		h7 = self.addHost( 'h7' )	
		h8 = self.addHost( 'h8' )	
		h9 = self.addHost( 'h9' )	
		h10 = self.addHost( 'h10' )	
		h11 = self.addHost( 'h11' )	
		h12 = self.addHost( 'h12' )	
		h13 = self.addHost( 'h13' )	
		h14 = self.addHost( 'h14' )	
		h15 = self.addHost( 'h15' )	
		# add switches
		s1 = self.addSwitch( 's1' )
		s2 = self.addSwitch( 's2' )
		s3 = self.addSwitch( 's3' )
		s4 = self.addSwitch( 's4' )
		s5 = self.addSwitch( 's5' )
		s6 = self.addSwitch( 's6' )
		# add links
		self.addLink( s1, s2 )
		self.addLink( s2, s3 )
		self.addLink( s2, s4 )
		self.addLink( s4, s5 )
		self.addLink( s4, s6 )
		self.addLink( s1, h1 )
		self.addLink( s1, h2 )
		self.addLink( s1, h3 )
		self.addLink( s2, h4 )
		self.addLink( s2, h5 )
		self.addLink( s2, h6 )
		self.addLink( s3, h7 )
		self.addLink( s3, h8 )
		self.addLink( s3, h9 )
		self.addLink( s5, h10 )
		self.addLink( s5, h11 )
		self.addLink( s5, h12 )
		self.addLink( s6, h13 )
		self.addLink( s6, h14 )
		self.addLink( s6, h15 )
	
topos = { 'mytopo': ( lambda: MyTopo() ) }
