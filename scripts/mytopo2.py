# --custom=mytopo.py --topo=mytopo

from mininet.topo import Topo
class MyTopo( Topo ):
	"Simple topology example."
	
	def __init__( self ):
		"Create custom topo."
		"""
			[s1]         [s2]
			/  \         /  \
		       /    \       /    \
		     [h1]   [h2]  [h3]  [h4]
		"""

		Topo.__init__(self)
		# add hosts
		h1 = self.addHost( 'h1' )	
		h2 = self.addHost( 'h2' )	
		h3 = self.addHost( 'h3' )	
		h4 = self.addHost( 'h4' )	
		# add switches
		s1 = self.addSwitch( 's1' )
		s2 = self.addSwitch( 's2' )
		# add links
		self.addLink( s1, h1 )
		self.addLink( s1, h2 )
		self.addLink( s2, h3 )
		self.addLink( s2, h4 )
	
topos = { 'mytopo': ( lambda: MyTopo() ) }
