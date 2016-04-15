/////////////////////////////////////////////////////////////////////////////
//	Class Set Owner Header
/////////////////////////////////////////////////////////////////////////////
	m_pOwner		= pOwner;

	if (m_pOwner)
	{
		m_pDevCmn		= m_pOwner->m_pDevCmn;
		m_pTranCmn		= m_pOwner->m_pTranCmn;

		if (m_pOwner->m_pDevCmn)
		{
			m_pProfile		= m_pOwner->m_pDevCmn->m_pProfile;
			m_pNetWork		= m_pOwner->m_pDevCmn->m_pNetWork;
			
			m_pSnd			= m_pOwner->m_pDevCmn->m_pSnd;
		}
	}
