from mininet.topo import Topo
class MyTopo( Topo ):
	"Simple topology example."
	
	def __init__( self ):
		"Create custom topo."
		Topo.__init__(self)
		# add hosts
		left_host1 = self.addHost( 'h1' )	
		left_host2 = self.addHost( 'h2' )	
		right_host1 = self.addHost( 'h3' )	
		right_host2 = self.addHost( 'h4' )	
		# add switches
		left_switch = self.addSwitch( 's1' )
		right_switch = self.addSwitch( 's2' )
		# add links
		self.addLink( left_switch, left_host1 )
		self.addLink( left_switch, left_host2 )
		self.addLink( right_switch, right_host1 )
		self.addLink( right_switch, right_host2 )
	
topos = { 'mytopo': ( lambda: MyTopo() ) }
