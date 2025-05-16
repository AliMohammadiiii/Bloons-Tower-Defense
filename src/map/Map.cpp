#include "Map.hpp"

TileType Map::at(int r,int c) const {
    switch(m_grid[r][c]){
        case MAP::Start: return TileType::Start;
        case MAP::Finish: return TileType::Finish;
        case MAP::Path: return TileType::Path;
        default : return TileType::Buildable;
    }
}

bool Map::inBounds(int r,int c) const{
    return r>=0 && r<(int)m_grid.size() &&
           c>=0 && c<(int)m_grid[0].size();
}

Map::Map(const std::string& fn){ parseFile(fn); tracePath(); }

void Map::parseFile(const std::string& fn){
    std::ifstream in(fn);
    if(!in) throw std::runtime_error("Cannot open map "+fn);
    std::string line;
    while(std::getline(in,line)){
        if(!line.empty()) m_grid.push_back(line);
    }
    if(m_grid.empty()) throw std::runtime_error("Empty map!");

    for(int r=0;r<(int)m_grid.size();++r)
        for(int c=0;c<(int)m_grid[r].size();++c){
            if(m_grid[r][c]=='S') m_start={r,c};
            else if(m_grid[r][c]=='F') m_finish={r,c};
        }
    if(m_start.x<0||m_finish.x<0)
        throw std::runtime_error("Map needs exactly one S and F");
}

void Map::tracePath()
{
    auto dir = sf::Vector2i{0,0};          
    sf::Vector2i cur = m_start;
    m_path.push_back(cur);

    const int dr[4]={-1,0,1,0}, dc[4]={0,1,0,-1};

    for(int k=0;k<4;++k){
        int nr=cur.x+dr[k], nc=cur.y+dc[k];
        if(inBounds(nr,nc) && (m_grid[nr][nc]==MAP::Path || m_grid[nr][nc]==MAP::Finish)){
            dir={dr[k],dc[k]}; break;
        }
    }

    while(cur!=m_finish)
    {
        sf::Vector2i ahead = {cur.x+dir.x, cur.y+dir.y};

        if(inBounds(ahead.x,ahead.y) &&
           (m_grid[ahead.x][ahead.y]==MAP::Path||m_grid[ahead.x][ahead.y]==MAP::Finish))
        {
            cur=ahead;
        } else {
            bool turned=false;
            for(int rot=-1; rot<=1; rot+=2){         
                int k = ( (std::find_if(dr,dr+4,[&](int v){return v==dir.x;})
                           -dr + rot + 4) % 4 );
                sf::Vector2i nxt={cur.x+dr[k],cur.y+dc[k]};
                if(inBounds(nxt.x,nxt.y) &&
                   (m_grid[nxt.x][nxt.y]==MAP::Path||m_grid[nxt.x][nxt.y]==MAP::Finish) &&
                   std::find(m_path.begin(),m_path.end(),nxt)==m_path.end())
                { dir={dr[k],dc[k]}; cur=nxt; turned=true; break; }
            }
            if(!turned) break; 
        }
        m_path.push_back(cur);
    }
}
