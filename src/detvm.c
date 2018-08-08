/*
*  Copyright (c) 2018 Savens Liu
*
*  The original has been patented, Open source is not equal to open rights. 
*  Anyone can clone, download, learn and discuss for free.  Without the permission 
*  of the copyright owner or author, it shall not be merged, published, licensed or sold. 
*  The copyright owner or author has the right to pursue his responsibility.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*/

#include    "tvm.h"

/*************************************************************************************************
    macro
 *************************************************************************************************/
#define     DEBUG_HEAD_INFO     1
#define     DEBUG_UNIQ_IDEX     2
#define     DEBUG_GROP_IDEX     4
#define     DEBUG_GROP_LIST     8
#define     DEBUG_DATA_LIST     16  
#define     DEBUG_IDEX_TREE     32
#define     DEBUG_IDEX_DATA     64
#define     DEBUG_IDEX_DALL     128
#define     DEBUG_ALLD_INFO     (DEBUG_HEAD_INFO|DEBUG_UNIQ_IDEX|DEBUG_GROP_IDEX|DEBUG_GROP_LIST|DEBUG_DATA_LIST)

/*************************************************************************************************
   function
 *************************************************************************************************/
extern void    vPrintHex(char *s, long lIdx, bool bf);

/*************************************************************************************************
    description：debug tree node
    parameters:
        pvData                     --memory address
        pstTree                    --tree node
    return:
  *************************************************************************************************/
void    vDebugTree(void *pvData, SHTree *pstTree)
{

    if(!pvData || !pstTree) return ;

    fprintf(stderr, "DugTree:%p-SePos:[%8ld], Idx(%ld):[%15s](%2ld), Color[%ld], lSePos:[%4ld], "
        "lParent[%4ld], left[%4ld], right[%4ld]\n" , (char *)pstTree, (void *)pstTree - pvData,
        pstTree->m_lIdx, pstTree->m_szIdx, pstTree->m_lData, pstTree->m_eColor, pstTree->m_lSePos,
        pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);

    if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
        return ;

    if(NODE_NULL != pstTree->m_lLeft)
        vDebugTree(pvData, (SHTree *)pGetNode(pvData, pstTree->m_lLeft));

    if(NODE_NULL != pstTree->m_lRight)
        vDebugTree(pvData, (SHTree *)pGetNode(pvData, pstTree->m_lRight));
}

/*************************************************************************************************
    description：debug table
    parameters:
        pvData                     --memory address
        pstTree                    --tree node
    return:
  *************************************************************************************************/
void    vDebugTable(TABLE t, long eType)
{
    long    i = 0, j = 0;
    RunTime *pstRun = NULL;
    TblKey  *pstKey = NULL;
    SHTree  *pstTree = NULL;
    SHList  *pstList = NULL;
    SHTruck *pstTruck = NULL;
    TIndex  *pstIndex = NULL;
    SATvm   *pstSavm = (SATvm *)pGetSATvm();

    if(NULL == (pstSavm = (SATvm *)pInitSATvm(t)))
    {
        fprintf(stderr, "initial table (%d) failed\n", t);
        return ;
    }

    if(NULL == (pstRun = pInitHitTest(pstSavm, pstSavm->tblName)))
    {
        fprintf(stderr, "hit test table (%d) failed, err:(%d)(%s)\n", t,pstSavm->m_lErrno, 
            sGetTError(pstSavm->m_lErrno));
        return ;
    }

    if(eType & DEBUG_HEAD_INFO)
    {
        fprintf(stdout, "\n---------------------------- TABLE HEAND INFO ----------------------"
            "----------\n");
        fprintf(stdout, "TABLE:%9u, extern:%10ld, NAME:%s\t\nSHTree:%8ld, SHList:%10ld, "
            "TblDef:%11ld\nGroup:%9ld, MaxRow:%10ld, Valid:%12ld\nlNodeNil:%6ld, lIType:%10d, "
            "Table:%12ld\nIdxLen:%8ld, TreePos:%9ld, TreeRoot:%9ld\nGrpLen:%8ld, GroupPos:%8ld, "
            "GroupRoot:%8ld\nData:%10ld, ReSize:%10ld, Truck:%12ld\nListPos:%7ld, ListOfs:%9ld, "
            "ExSeQ:%12lld\n", 
            ((TblDef *)pGetTblDef(t))->m_table, ((TblDef *)pGetTblDef(t))->m_lExtern, 
            ((TblDef *)pGetTblDef(t))->m_szTable, sizeof(SHTree), sizeof(SHList), sizeof(TblDef), 
            ((TblDef *)pGetTblDef(t))->m_lGroup, ((TblDef *)pGetTblDef(t))->m_lMaxRow, 
            ((TblDef *)pGetTblDef(t))->m_lValid, ((TblDef *)pGetTblDef(t))->m_lNodeNil,
            ((TblDef *)pGetTblDef(t))->m_lIType, ((TblDef *)pGetTblDef(t))->m_lTable, 
            ((TblDef *)pGetTblDef(t))->m_lIdxLen, ((TblDef *)pGetTblDef(t))->m_lTreePos, 
            ((TblDef *)pGetTblDef(t))->m_lTreeRoot, ((TblDef *)pGetTblDef(t))->m_lGrpLen, 
            ((TblDef *)pGetTblDef(t))->m_lGroupPos, ((TblDef *)pGetTblDef(t))->m_lGroupRoot, 
            ((TblDef *)pGetTblDef(t))->m_lData, ((TblDef *)pGetTblDef(t))->m_lReSize, 
            ((TblDef *)pGetTblDef(t))->m_lTruck, ((TblDef *)pGetTblDef(t))->m_lListPos, 
            ((TblDef *)pGetTblDef(t))->m_lListOfs, ((TblDef *)pGetTblDef(t))->m_lExSeQ);
        fprintf(stdout, "--------------------------------------------------------------------"
            "----------\n");

        pstTree = &((TblDef *)pGetTblDef(t))->m_stNil;
        fprintf(stdout, ">>NIL:[%8ld], Idx:[%s](%ld)(%ld), Color[%ld], lSePos:[%4ld], lParent[%4ld]"
            ", left[%4ld], right[%4ld]\n" , (void *)pstTree - (void *)pGetTblDef(t), pstTree->m_szIdx, 
            pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor, pstTree->m_lSePos, pstTree->m_lParent, 
            pstTree->m_lLeft, pstTree->m_lRight); 

        fprintf(stdout, "\n--------------------------------UNIQ INDEX FIELD--------------"
            "----------------\n");
        for(i = 0, pstKey = pGetTblIdx(t); i < lGetIdxNum(t); i ++)
        {
            fprintf(stdout, "From:%4ld, len:%3ld, attr:%ld, IsPk:%ld, field:%s\n", pstKey[i].m_lFrom,
                pstKey[i].m_lLen, pstKey[i].m_lAttr, pstKey[i].m_lIsPk, pstKey[i].m_szField);
        }

        fprintf(stdout, "\n-------------------------------CHECK INDEX FIELD--------------"
            "----------------\n");
        for(i = 0, pstKey = pGetTblGrp(t); i < lGetGrpNum(t); i ++)
        {
            fprintf(stdout, "From:%4ld, len:%3ld, attr:%ld, IsPk:%ld, field:%s\n", pstKey[i].m_lFrom,
                pstKey[i].m_lLen, pstKey[i].m_lAttr, pstKey[i].m_lIsPk, pstKey[i].m_szField);
        }

        fprintf(stdout, "\n_______________________________  TABLE FIELD ____________________"
            "_____________\n");
        for(i = 0, pstKey = pGetTblKey(t); i < lGetFldNum(t); i ++)
        {
            fprintf(stdout, "pos:%4ld, len:%3ld, atr:%ld, pk:%ld, fld:%-20s, als:%s\n", 
                pstKey[i].m_lFrom, pstKey[i].m_lLen, pstKey[i].m_lAttr, pstKey[i].m_lIsPk, 
                pstKey[i].m_szField, pstKey[i].m_szAlias);
        }
    }                

    if(eType & DEBUG_UNIQ_IDEX && (((TblDef *)pstRun->m_pvAddr)->m_lIType & UNQIUE))
    {
        fprintf(stdout, "\n===================================UNIQUE_INDEX====================="
            "==============lValid(%ld)=========================\n", lGetTblValid(t));
        if(eType & DEBUG_IDEX_DALL)
        {
            for(i = 0; i < lGetTblRow(t) < 3 ? 3 : lGetTblRow(t); i ++)
            {
                pstTree = (SHTree *)(pstRun->m_pvAddr + lGetIdxPos(t) + i * sizeof(SHTree));
                vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
                fprintf(stdout, "NODE:[%ld](%02ld), Idx:[%4s](%ld)(%2ld), Color[%ld], lSePos:[%4ld], "
                    "lParent[%4ld], left[%4ld], right[%4ld]\n" , (void *)pstTree - pstRun->m_pvAddr, 
                     i, pstTree->m_szIdx, pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor,
                    pstTree->m_lSePos, pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
            }
        }
        else
        {
            for(i = 0; i < (lGetTblValid(t) < 3 ? 3 : lGetTblValid(t)); i ++)
            {
                pstTree = (SHTree *)(pstRun->m_pvAddr + lGetIdxPos(t) + i * sizeof(SHTree));
//                if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
//                    continue;

                vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
                fprintf(stdout, "NODE:[%6ld]->(%02ld), Idx:[%15s](%6ld), Color[%ld], lSePos:[%4ld], "
                    "lParent[%4ld], left[%4ld], right[%4ld]\n" , (void *)pstTree - pstRun->m_pvAddr, i, 
                    pstTree->m_szIdx, pstTree->m_lData, pstTree->m_eColor, pstTree->m_lSePos,
                    pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
            }
        }
    }

    if(eType & DEBUG_GROP_IDEX && (((TblDef *)pstRun->m_pvAddr)->m_lIType & NORMAL))
    {
        fprintf(stdout, "\n===================================INDEX_GROUP====================="
            "==============Valid:%ld, Group:%ld================\n", lGetTblValid(t), lGetTblGroup(t));
        if(eType & DEBUG_IDEX_DALL)
        {
            for(i = 0; i < lGetTblRow(t); i ++)
            {
                pstTree = (SHTree *)(pstRun->m_pvAddr + lGetGrpPos(t) + i * sizeof(SHTree));
                vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
                fprintf(stdout, "NODE:[%ld](%02ld), Idx:[%4s](%ld)(%2ld), Color[%ld], lSePos:[%4ld],"
                    " lParent[%4ld], left[%4ld], right[%4ld]\n" , (void *)pstTree - pstRun->m_pvAddr, 
                    i, pstTree->m_szIdx, pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor,
                    pstTree->m_lSePos, pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
            }
        }
        else
        {
            for(i = 0; i < (lGetTblValid(t) < 3 ? 3 : lGetTblValid(t)); i ++)
            {
                pstTree = (SHTree *)(pstRun->m_pvAddr + lGetGrpPos(t) + i * sizeof(SHTree));
//                if(SELF_POS_UNUSE == pstTree->m_lSePos || NODE_NULL == pstTree->m_lSePos)
//                    continue;
            
                vPrintHex(pstTree->m_szIdx, pstTree->m_lIdx, 0);
                fprintf(stdout, "NODE:[%ld](%02ld), Idx:[%4s](%ld)(%2ld), Color[%ld], lSePos:[%4ld],"
                    " lParent[%4ld], left[%4ld], right[%4ld]\n" , (void *)pstTree - pstRun->m_pvAddr, 
                    i, pstTree->m_szIdx, pstTree->m_lIdx, pstTree->m_lData, pstTree->m_eColor,
                    pstTree->m_lSePos, pstTree->m_lParent, pstTree->m_lLeft, pstTree->m_lRight);
            }
        }
    }

    if(eType & DEBUG_GROP_LIST && (((TblDef *)pstRun->m_pvAddr)->m_lIType & NORMAL))
    {
        fprintf(stdout, "\n=================================INDEX_LIST========================"
            "==============Valid(%ld)=============\n", lGetTblValid(t));
        if(eType & DEBUG_IDEX_DALL)
        {
            for(i = 0, j = lGetListOfs(t); i < lGetTblRow(t); i ++)
            {
                pstList = (SHList *)(pstRun->m_pvAddr + j + i * sizeof(SHList));
                fprintf(stdout, "LIST:[%8ld][%02ld], lSePos:[%4ld], lData[%8ld], Node[%8ld], "
                    "Next[%8ld], Last[%8ld]\n" , (void *)pstList - pstRun->m_pvAddr, i, 
                    pstList->m_lPos, pstList->m_lData, pstList->m_lNode, pstList->m_lNext, 
                    pstList->m_lLast);
            }
        }
        else
        {
            for(i = 0, j = lGetListOfs(t); i < (lGetTblValid(t) < 3 ? 3 : lGetTblValid(t)); i ++)
            {
                pstList = (SHList *)(pstRun->m_pvAddr + j + i * sizeof(SHList));
//                if(SELF_POS_UNUSE == pstList->m_lPos)
//                    continue;
            
                fprintf(stdout, "LIST:[%8ld][%02ld], lSePos:[%4ld], lData[%8ld], Node[%8ld], "
                    "Next[%8ld], Last[%8ld]\n" , (void *)pstList - pstRun->m_pvAddr, i, 
                    pstList->m_lPos, pstList->m_lData, pstList->m_lNode,
                    pstList->m_lNext, pstList->m_lLast);
            }
        }
    }

    if(eType & DEBUG_IDEX_TREE)
    {
        fprintf(stdout, "\n=================================TREE DEUBG========================"
            "==============Valid(%ld)=============\n", lGetTblValid(t));
        vDebugTree(pstRun->m_pvAddr, pGetNode(pstRun->m_pvAddr, lGetIdxRoot(t)));
    }

    if(eType & DEBUG_IDEX_DATA)
    {
        fprintf(stdout, "\n===================================UNIQUE_INDEX====================="
            "==============lValid(%ld)=========================\n", lGetTblValid(t));
        for(i = 0; i < lGetTblRow(t); i ++)
        {
            pstTruck = (void *)(pstRun->m_pvAddr + lGetTblData(t) + i * lGetRowTruck(t));
            fprintf(stdout, "SePos[%ld][%X]\n", (void *)pstTruck - pstRun->m_pvAddr, pstTruck->m_chTag);
            vPrintHex(pstTruck->m_pvData, lGetRowSize(t), 0);
        }
    }

    vTblDisconnect(pstSavm, pstSavm->tblName);
    fprintf(stdout, "================================================================="
            "==============\n");
}

/*************************************************************************************************
    description：get action
    parameters:
    return:
  *************************************************************************************************/
void    vGetAction(char *s, int *plAction)
{
    int     i, nLen;

    if(!s)    return ;

    for(i = 0, nLen = strlen(s); i < nLen; i ++)
    {
        switch(s[i])
        {
        case 'h':
            *plAction |= DEBUG_HEAD_INFO;                
            break;
        case 'u':
            *plAction |= DEBUG_UNIQ_IDEX;                
            break;
        case 'g':
            *plAction |= DEBUG_GROP_IDEX;                
            break;
        case 'l':
            *plAction |= DEBUG_GROP_LIST;                
            break;
        case 'd':
            *plAction |= DEBUG_DATA_LIST;                
            break;
        case 't':
            *plAction |= DEBUG_IDEX_TREE;                
            break;
        case 'e':
            *plAction |= DEBUG_IDEX_DALL;                
            break;
        case 'a':
            *plAction |= DEBUG_IDEX_DATA;                
            break;
        default:
            break;
        }
    }
}

/*************************************************************************************************
    description：print func
    parameters:
    return:
  *************************************************************************************************/
void    vPrintFunc(char *s)
{
    fprintf(stdout, "\nUsage:\t%s -[t][hugldtui]\n", s);
    fprintf(stdout, "\t-t\t\t--table\n");
    fprintf(stdout, "\t-p[hugldta]\t--debug\n");
    fprintf(stdout, "\n");
}

/*************************************************************************************************
    description：main
    parameters:
    return:
  *************************************************************************************************/
int     main(int argc, char *argv[])
{
    TABLE   t;
    char    szCom[256];
    int     iChoose = 0, iAction = 0;

    memset(szCom, 0, sizeof(szCom));
    while(-1 != (iChoose = getopt(argc, argv, "t:p::v?::")))
    {
        switch(iChoose)
        {
        case    't':
            iAction |= 1;
            t = atol(optarg);
            break;
        case    'p':
            iAction |= 4;
            if(!optarg)
                szCom[0] = 'h';
            else
                strcpy(szCom, optarg);
            break;
        case    'v':
        case    '?':
        default:
            vPrintFunc(basename(argv[0]));
            return RC_FAIL;
        }
    }

    iChoose = 0;
    if(5 == iAction)
    {
        vGetAction(szCom, &iChoose);
        vDebugTable(t, iChoose);
        return RC_SUCC;
    }

    vPrintFunc(basename(argv[0]));

    return RC_SUCC;
}

/*************************************************************************************************
    code end
  *************************************************************************************************/
